/*
** main.c for irc in /home/scutar_n/rendu/PSU/myirc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Tue May 30 11:21:20 2017 Nathan Scutari
** Last update Sun Jun 11 15:55:43 2017 Nathan Scutari
*/

#include "server.h"
#include "ringbuffer.h"

int	print_err(char *msg, int ret)
{
  write(2, msg, strlen(msg));
  return (ret);
}

int	is_ipaddress(char *str)
{
  int	i;

  i = -1;
  while (str[++i])
    {
      if ((str[i] < '0' || str[i] > '9') &&
	  str[i] != '.' && str[i] != ',')
	return (0);
    }
  return (1);
}

int	connect_client(t_client *client, t_inf *inf)
{
  if (!client->registered && client->nick && client->user
      && client->ping.idle == 0)
    {
      client->registered = 1;
      dprintf(client->fd, ":%s 001 %s :Welcome %s!%s@%s\r\n", inf->hostname, client->nick,
	      client->nick, first_arg(client->user), client->hostname);
      dprintf(client->fd, ":%s 002 %s :Your host is %s\r\n", inf->hostname, client->nick, inf->hostname);
      dprintf(client->fd, ":%s 003 %s :This server was created on %s\r\n", inf->hostname, client->nick, CREATION_DATE);
      dprintf(client->fd, ":%s 004 %s :...\r\n", inf->hostname, client->nick);
      dprintf(client->fd, ":%s 005 %s :Try server \"none\" instead\r\n", inf->hostname, client->nick);
      dprintf(client->fd, ":%s MODE %s :+iwx\r\n", client->nick, client->nick);
    }
  return (0);
}

int	send_to_client(char *msg, t_client *client)
{
  dprintf(client->fd, ":%s %s\r\n", HOSTNAME, msg);
  return (0);
}

void	send_ping(t_client *client)
{
  client->ping.timer = time(NULL);
  client->ping.idle = 1;
  dprintf(client->fd, "PING :%s\r\n", HOSTNAME);
}

int	ping_command(t_client *client, t_inf *inf, char *arg)
{
  int	ret;
  int	pos;
  int	i;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 you have not registered\r\n", inf->hostname);
      return (0);
    }
  pos = first_arg_pos(arg);
  if ((ret = get_arg_pos(&arg[pos], 2)) == -1)
    {
      dprintf(client->fd, ":%s 409 %s missing argument\r\n",
	      inf->hostname, (client->nick ? client->nick : ""));
      return (0);
    }
  pos += ret;
  i = pos;
  while (arg[++i] && arg[i] != ' ');
  arg[i] = '\0';
  dprintf(client->fd, ":%s PONG %s :%s\r\n",
	  inf->hostname, inf->hostname,
	  (arg[pos] == ':' ? &arg[pos + 1] : &arg[pos]));
  return (0);
}

int	pong_command(t_client *client, t_inf *inf, char *arg)
{
  int	pos;
  char	*str;
  int	i;

  if (client->ping.idle == 0)
    return (0);
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    return (0);
  if (str[pos] == ':')
    ++pos;
  i = pos;
  while (str[++i] && str[i] != ' ');
  str[i] = '\0';
  if (strcmp(&str[pos], inf->hostname) == 0)
    client->ping.idle = 0;
  return (connect_client(client, inf));
}

int	add_chan_to_client(t_channel *chan, t_client *client)
{
  t_join	*tmp;
  t_join	*new_join;

  if ((new_join = malloc(sizeof(t_join))) == NULL)
    return (1);
  new_join->chan = chan;
  new_join->next = NULL;
  if (client->chan == NULL)
    client->chan = new_join;
  else
    {
      tmp = client->chan;
      while (tmp && tmp->next)
	tmp = tmp->next;
      tmp->next = new_join;
    }
  return (0);
}

int	add_client_to_chan(t_client *client, t_channel *chan, int admin)
{
  t_member	*tmp;
  t_member	*parse;

  if ((tmp = malloc(sizeof(t_member))) == NULL)
    return (1);
  tmp->fd = client->fd;
  tmp->admin = admin;
  tmp->next = NULL;
  if (chan->member == NULL)
    chan->member = tmp;
  else
    {
      parse = chan->member;
      while (parse && parse->next)
	{
	  if (parse->fd == client->fd ||
	      parse->next->fd == client->fd)
	    {
	      free(tmp);
	      return (1);
	    }
	  parse = parse->next;
	}
      parse->next = tmp;
    }
  return (add_chan_to_client(chan, client));
}

void	inform_all_join(t_channel *chan, t_client *client)
{
  t_member	*tmp;

  tmp = chan->member;
  while (tmp)
    {
      dprintf(tmp->fd, ":%s!%s@%s JOIN %s\r\n",
	      client->nick, first_arg(client->user),
	      client->hostname, chan->name);
      tmp = tmp->next;
    }
}

void	inform_client_join(t_channel *chan, t_client *client, t_inf *inf)
{
  t_client	*tmp;
  t_member	*parse;

  dprintf(client->fd, ":%s 353 %s = %s ", HOSTNAME, client->nick, chan->name);
  parse = chan->member;
  if ((tmp = get_client(parse->fd, inf)) != NULL)
    dprintf(client->fd, ":%s%s", (parse->admin == 1 ? "@" : ""),
	    tmp->nick);
  parse = parse->next;
  while (parse)
    {
      if ((tmp = get_client(parse->fd, inf)) != NULL)
	dprintf(client->fd, " %s%s", (parse->admin == 1 ? "@" : ""),
		tmp->nick);
      parse = parse->next;
    }
  dprintf(client->fd, "\r\n:%s 366 %s %s :End of /NAMES list.\r\n",
	 HOSTNAME, client->nick, chan->name);
}

int	join_chan(t_inf *inf, t_client *client, char *chan_name)
{
  int		admin;
  t_channel	*chan;

  if (chan_name[0] != '#' && chan_name[0] != '&')
    {
      dprintf(client->fd, ":%s 403 %s :No such channel\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  admin = 0;
  if ((chan = find_chan(chan_name, inf)) == NULL &&
      (admin = 1 == 1) &&
      (chan = create_chan(chan_name, inf)) == NULL)
    return (-1);
  if (add_client_to_chan(client, chan, admin))
    return (1);
  inform_all_join(chan, client);
  inform_client_join(chan, client, inf);
  return (0);
}

int	names_command(t_client *client, t_inf *inf, char *arg)
{
  int	pos;
  char	*str;
  int	i;
  char	*chan;
  t_channel	*channel;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 You have not registered\r\n", HOSTNAME);
      return (0);
    }
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    {
      dprintf(client->fd, ":%s 366 %s * :End of /NAMES list\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  str = &str[pos];
  i = -1;
  while (str[++i] && str[i] != ' ');
  str[i] = '\0';
  while ((chan = strsep(&str, ",")))
    {
      if ((channel = find_chan(chan, inf)))
	inform_client_join(channel, client, inf);
      else
	dprintf(client->fd, ":%s 401 %s %s :No such channel\r\n",
		HOSTNAME, client->nick, chan);
    }
  return (0);
}

int	join_command(t_client *client, t_inf *inf, char *arg)
{
  char	*str;
  int	pos;
  char	*chan;
  int	i;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 You have not registered\r\n", inf->hostname);
      return (0);
    }
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    {
      dprintf(client->fd, ":%s 461 %s :Not enough parameters\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  str = &str[pos];
  i = -1;
  while (str[++i] && str[i] != ' ');
  str[i] = '\0';
  while ((chan = strsep(&str, ",")))
    join_chan(inf, client, chan);
  return (0);
}

int	send_to_chan(t_client *client, t_channel *chan,
		     char *txt)
{
  t_member	*tmp;

  printf("send to chan\n");
  tmp = chan->member;
  while (tmp)
    {
      printf("parsing\n");
      if (tmp->fd != client->fd)
	{
	  printf("found\n");
	  dprintf(tmp->fd, ":%s!%s@%s PRIVMSG %s :%s\r\n",
		  client->nick, first_arg(client->user), client->hostname,
		  chan->name, (txt[0] == ':' ? &txt[1] : txt));
	}
      tmp = tmp->next;
    }
  return (0);
}

void	cancel_requests(t_client *tmp)
{
  if (tmp->request.send)
    {
      tmp->request.send->request.get = NULL;
      tmp->request.send = NULL;
    }
  if (tmp->request.get)
    {
      tmp->request.get->request.send = NULL;
      tmp->request.get = NULL;
    }
}

int	send_private(t_client *from, t_client *to, char *txt)
{
  dprintf(to->fd, ":%s!%s@%s PRIVMSG %s :%s\r\n",
	  from->nick, first_arg(from->user), from->hostname,
	  to->nick, (txt[0] == ':' ? &txt[1] : txt));
  return (0);
}

int	sendprivmsg(t_client *client, char *to, char *str, t_inf *inf)
{
  t_channel	*chan;
  t_client	*tmp;

  if ((chan = find_chan(to, inf)))
    send_to_chan(client, chan, str);
  else if ((tmp = find_client_by_name(to, NULL, inf)))
    send_private(client, tmp, str);
  else
    dprintf(client->fd, ":%s 401 %s :No such nick/channel\r\n",
	    HOSTNAME, client->nick);
  return (0);
}

void	leave_chan(t_client *client, char *chan, int msg, t_inf *inf)
{
  t_channel	*tmp;
  char		buff[200];

  if ((tmp = find_chan(chan, inf)) == NULL)
    {
      dprintf(client->fd, ":%s 403 %s :No such channel\r\n",
	       HOSTNAME, client->nick);
      return ;
    }
  if (msg == -1)
    sprintf(buff, ":%s!%s@%s PART %s\r\n", client->nick,
	    first_arg(client->user), client->hostname,
	    tmp->name);
  else
    sprintf(buff, ":%s!%s@%s PART %s :%s", client->nick,
	    first_arg(client->user), client->hostname,
	    tmp->name, (chan[msg] == ':' ? &chan[msg + 1] : &chan[msg]));
  send_custom_to_chan(client, tmp, buff);
  dprintf(client->fd, "%s", buff);
  delete_client_from_chan(client, tmp, inf);
}

int	part_command(t_client *client, t_inf *inf, char *arg)
{
  char	*str;
  char	*chan;
  int	pos;
  int	msg;
  int	i;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 PART :You have not registered\r\n",
	      HOSTNAME);
      return (0);
    }
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    {
      dprintf(client->fd, ":%s 461 %s PART :Not enough parameters\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  msg = get_arg_pos(str, 3);
  str = &str[pos];
  i = -1;
  while (str[++i] && str[i] != '\0');
  str[i] = '\0';
  while ((chan = strsep(&str, ",")))
    leave_chan(client, chan, msg, inf);
  return (0);
}

int	privmsg_command(t_client *client, t_inf *inf, char *arg)
{
  char	*str;
  char	*msg;
  char	*to;
  int	pos;
  int	txt;
  int	i;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 PRIVMSG :You have not registered\r\n",
	      HOSTNAME);
      return (0);
    }
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    {
      dprintf(client->fd, ":%s 411 %s :No recipient given\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  str = &str[pos];
  if ((txt = get_arg_pos(str, 2)) == -1)
    {
      dprintf(client->fd, "%s 412 %s :No text to send\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  i = -1;
  while (str[++i] && str[i] != ' ');
  str[i] = '\0';
  if ((msg = strdup(&str[txt])) == NULL)
    return (-1);
  while ((to = strsep(&str, ",")))
    sendprivmsg(client, to, msg, inf);
  free(msg);
  return (0);
}

void	inform_quit(t_client *to, t_client *from, char *msg, int pos)
{
  if (pos == -1)
    {
      dprintf(to->fd, ":%s!%s@%s QUIT :Quit: %s\r\n", from->nick,
	      first_arg(from->user), from->hostname, from->nick);
    }
  else
    {
      dprintf(to->fd, ":%s!%s@%s QUIT :Quit: %s\r\n", from->nick,
	      first_arg(from->user), from->hostname, &msg[pos]);
    }
}

int	quit_command(t_client *client, t_inf *inf, char *arg)
{
  t_client	*tmp;
  char		*str;
  int		msg;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 QUIT :You have not registered\r\n",
	      HOSTNAME);
      return (0);
    }
  str = &arg[first_arg_pos(arg)];
  msg = get_arg_pos(str, 2);
  tmp = inf->client;
  while (tmp)
    {
      if (tmp->fd != client->fd && in_same_channel(tmp, client))
	inform_quit(tmp, client, str, msg);
      tmp = tmp->next;
    }
  disconnect_client(client, str, msg, inf);
  return (1);
}

int	list_command(t_client *client, t_inf *inf, UNUSED char *arg)
{
  t_channel	*tmp;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 LIST :You have not regisered\r\n",
	      HOSTNAME);
      return (0);
    }
  dprintf(client->fd, ":%s 321 %s Channel :Users Name\r\n",
	  HOSTNAME, client->nick);
  tmp = inf->channel;
  while (tmp)
    {
      dprintf(client->fd, ":%s 322 %s %s %d :\r\n", HOSTNAME,
	      client->nick, tmp->name, count_users(tmp));
      tmp = tmp->next;
    }
  dprintf(client->fd, ":%s 323 %s :End of /LIST\r\n", HOSTNAME,
	  client->nick);
  return (0);
}

int	users_command(t_client *client, t_inf *inf, UNUSED char *arg)
{
  t_client	*tmp;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 USERS :You have not regisered\r\n",
	      HOSTNAME);
      return (0);
    }
  dprintf(client->fd, ":%s 392 %s Users :UserId Host\r\n",
	  HOSTNAME, client->nick);
  tmp = inf->client;
  while (tmp)
    {
      if (tmp->nick)
	dprintf(client->fd, ":%s 393 %s %s :%s %s\r\n", HOSTNAME,
		client->nick, tmp->nick, first_arg(tmp->user),
		tmp->hostname);
      tmp = tmp->next;
    }
  dprintf(client->fd, ":%s 394 %s :End of /USERS\r\n",
	  HOSTNAME, client->nick);
  return (0);
}

void	send_file(t_client *client, t_inf *inf, char *arg)
{
  int		i;
  int		pos;
  t_client	*tmp;

  if ((pos = get_arg_pos(arg, 3)) == -1 ||
      get_arg_pos(arg, 4) == -1 ||
      get_arg_pos(arg, 5) == -1)
    {
      dprintf(client->fd, ":%s 461 %s :Not enough arguments\r\n",
	      HOSTNAME, client->nick);
      return ;
    }
  i = pos;
  while (arg[++i] && arg[i] != ' ');
  arg[i] = '\0';
  if ((tmp = find_client_by_name(&arg[pos], client, inf)) == NULL)
    {
      dprintf(client->fd, ":%s 141 %s :This user does not exist\r\n",
	      HOSTNAME, client->nick);
      return ;
    }
  arg[i] = ' ';
  if (client->request.send)
    {
      dprintf(client->fd, ":%s 143 %s :A SEND request is already pending\r\n",
	      HOSTNAME, client->nick);
      return ;
    }
  if (tmp->request.get)
    {
      dprintf(client->fd,
	      ":%s 144 %s :Target already has a GET request pending\r\n",
	      HOSTNAME, client->nick);
    }
  tmp->request.get = client;
  client->request.send = tmp;
  dprintf(tmp->fd, ":%s!%s@%s %s\r\n", client->nick,
	  first_arg(client->user), client->hostname,
	  arg);
}

void	get_file(t_client *client, UNUSED t_inf *inf, UNUSED char *arg)
{
  if (client->request.get == NULL)
    {
      dprintf(client->fd, ":%s 142 %s :No pending file request\r\n",
	      HOSTNAME, client->nick);
      return ;
    }
  dprintf(client->request.get->fd, ":%s!%s@%s FILE GET\r\n",
	  client->nick, first_arg(client->user), client->hostname);
  client->request.get->request.send = NULL;
  client->request.get = NULL;
}

void	refuse_file(t_client *client, UNUSED t_inf *inf, UNUSED char *arg)
{
  if (client->request.get == NULL)
    {
      dprintf(client->fd, ":%s 142 %s :No pending file request\r\n",
	      HOSTNAME, client->nick);
      return ;
    }
  dprintf(client->request.get->fd, ":%s!%s@%s FILE REFUSE\r\n",
	  client->nick, first_arg(client->user), client->hostname);
  client->request.get->request.send = NULL;
  client->request.get = NULL;
}

int	file_command(t_client *client, t_inf *inf, char *arg)
{
  int	i;
  int	pos;
  char	save;

  if (client->registered == 0)
    {
      dprintf(client->fd, ":%s 451 FILE :You have not registered\r\n",
	      HOSTNAME);
      return (0);
    }
  if ((pos = get_arg_pos(arg, 2)) == -1)
    {
      dprintf(client->fd, ":%s 461 %s :Not enough arguments\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  i = pos;
  while (arg[++i] && arg[i] != ' ');
  save = arg[i];
  arg[i] = '\0';
  if (strcasecmp(&arg[pos], "SEND") == 0)
    {
      arg[i] = save;
      send_file(client, inf, arg);
    }
  else if (strcasecmp(&arg[pos], "GET") == 0)
    {
      arg[i] = save;
      get_file(client, inf, arg);
    }
  else if (strcasecmp(&arg[pos], "REFUSE") == 0)
    {
      refuse_file(client, inf, arg);
    }
  else
    {
      dprintf(client->fd, ":%s 140 %s :Wrong argument\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  return (0);
}

int	main(int ac, char **av)
{
  int	ret;
  t_inf	inf;

  if (ac != 2)
    return (print_err("Usage ./server [port]", 1));
  if ((inf.server = get_server_socket(av[1])) == -1 ||
      (inf.signal = init_signals()) == -1)
    return (1);
  if (listen(inf.server, 42) == -1)
    return (print_err("listen failed\n", 1));
  srand(time(NULL));
  inf.hostname = HOSTNAME;
  inf.client = NULL;
  inf.channel = NULL;
  ret = server_loop(&inf);
  delete_clients(&inf);
  close(inf.server);
  return (ret);
}
