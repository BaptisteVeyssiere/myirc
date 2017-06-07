/*
** main.c for irc in /home/scutar_n/rendu/PSU/myirc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Tue May 30 11:21:20 2017 Nathan Scutari
** Last update Wed Jun  7 22:02:35 2017 Nathan Scutari
*/

#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <sys/select.h>

#define CLIENT_HOST_SIZE	60
#define CREATION_DATE		"03/06/2017"
#define HOSTNAME		"irc.server.tek"
#define LOG_TIMEOUT_SEC		45
#define PING_DELAY		60
#define IDLE_TIMEOUT_SEC	120
#define UNUSED			__attribute__((unused))
#define RINGLENGTH		4096

typedef struct		s_ringbuffer
{
  char			data[RINGLENGTH];
  int			write_ptr;
  int			read_ptr;
}			t_ringbuffer;

typedef struct		s_ping
{
  char			idle;
  int			timer;
  char			first;
}			t_ping;

typedef struct		s_join
{
  struct s_channel	*chan;
  struct s_join		*next;
}			t_join;

typedef struct		s_client
{
  char			*user;
  char			*nick;
  char			*hostname;
  int			fd;
  t_ringbuffer		buff;
  t_ping		ping;
  int			registered;
  t_join		*chan;
  struct s_client	*next;
}			t_client;

typedef struct		s_member
{
  int			fd;
  char			admin;
  struct s_member	*next;
}			t_member;

typedef struct		s_channel
{
  char			*name;
  t_member		*member;
  struct s_channel	*next;
}			t_channel;

typedef struct		s_inf
{
  int			server;
  int			signal;
  char			*hostname;
  t_client		*client;
  t_channel		*channel;
}			t_inf;

int	print_err(char *msg, int ret)
{
  write(2, msg, strlen(msg));
  return (ret);
}

void	delete_chan_from_client(t_client *client, t_channel *chan)
{
  t_join	*tmp;
  t_join	*prev;

  tmp = client->chan;
  prev = NULL;
  while (tmp)
    {
      if (tmp->chan == chan)
	{
	  if (prev == NULL)
	    client->chan = tmp->next;
	  else
	    prev->next = tmp->next;
	  free(tmp);
	  return ;
	}
      prev = tmp;
      tmp = tmp->next;
    }
}

void	delete_client_from_chan(t_client *client, t_channel *chan)
{
  t_member	*tmp;
  t_member	*prev;

  tmp = chan->member;
  prev = NULL;
  while (tmp)
    {
      if (tmp->fd == client->fd)
	{
	  if (prev == NULL)
	    chan->member = tmp->next;
	  else
	    prev->next = tmp->next;
	  free(tmp);
	  delete_chan_from_client(client, chan);
	  return ;
	}
      prev = tmp;
      tmp = tmp->next;
    }
  return ;
}

int	get_server_socket(char *arg)
{
  struct sockaddr_in	s_in;
  struct protoent	*pe;
  int			i = -1;
  int			port;
  int			fd;

  while (arg[++i])
    if (arg[i] < '0' || arg[i] > '9')
      return (-1);
  port = atoi(arg);
  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(port);
  s_in.sin_addr.s_addr = INADDR_ANY;
  if (!(pe = getprotobyname("TCP")))
    return (print_err("getprotobybyname failed\n", -1));
  if ((fd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    return (print_err("socket failed\n", -1));
  if (bind(fd, (const struct sockaddr *)&s_in, sizeof(s_in)) == -1)
    {
      close(fd);
      return (print_err("bind failed\n", -1));
    }
  return (fd);
}

int	add_client(int client_fd, t_inf *inf)
{
  t_client	*tmp;
  t_client	*new_client;

  if ((new_client = malloc(sizeof(t_client))) == NULL)
    return (print_err("malloc failed\n", -1));
  new_client->user = NULL;
  new_client->nick = NULL;
  new_client->fd = client_fd;
  new_client->buff.write_ptr = 0;
  new_client->buff.read_ptr = 0;
  new_client->ping.timer = time(NULL);
  new_client->ping.idle = 0;
  new_client->ping.first = 0;
  new_client->next = NULL;
  new_client->registered = 0;
  new_client->hostname = NULL;
  new_client->chan = NULL;
  bzero(new_client->buff.data, RINGLENGTH);
  if (!(inf->client))
    inf->client = new_client;
  else
    {
      tmp = inf->client;
      while (tmp && tmp->next)
	tmp = tmp->next;
      tmp->next = new_client;
    }
  return (0);
}

t_client	*get_client(int fd, t_inf *inf)
{
  t_client	*tmp;

  tmp = inf->client;
  while (tmp)
    {
      if (tmp->fd == fd)
	return (tmp);
      tmp = tmp->next;
    }
  return (NULL);
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

int	get_clienthostname(t_client *client, struct sockaddr_in *s_in,
			   socklen_t size)
{
  if ((client->hostname = malloc(CLIENT_HOST_SIZE)) == NULL)
    return (-1);
  dprintf(client->fd, ":%s NOTICE * :*** Looking for hostname...\r\n", HOSTNAME);
  if ((getnameinfo((struct sockaddr*)s_in,
		   size, client->hostname, CLIENT_HOST_SIZE,
		   NULL, 0, NI_NOFQDN)) != 0)
    return (-1);
  if (is_ipaddress(client->hostname))
    {
      dprintf(client->fd, ":%s NOTICE * :*** Could not find your hostname",
	      HOSTNAME);
      dprintf(client->fd, "using your ip address instead (%s)\r\n",
	      client->hostname);
    }
  else
    dprintf(client->fd, ":%s NOTICE * :*** Found your hostname (%s)\r\n",
	    HOSTNAME, client->hostname);
  return (0);
}

int	accept_new_client(fd_set *set, t_inf *inf)
{
  t_client		*client;
  int			client_fd;
  socklen_t		size;
  struct sockaddr_in	s_in;

  size = sizeof(s_in);
  if ((client_fd = accept(inf->server, (struct sockaddr*)&s_in,
			  &size)) == -1)
    return (print_err("accept failed\n", -1));
  if ((add_client(client_fd, inf)) == -1)
    return (-1);
  if ((client = get_client(client_fd, inf)) == NULL ||
      get_clienthostname(client, &s_in, size) == -1)
    return (-1);
  FD_SET(client_fd, set);
  return (0);
}

int	read_socket(int fd, t_client *client)
{
  int		ret;
  char		buff[257];
  int		i;

  bzero(buff, 257);
  ret = read(fd, buff, 256);
  if (ret == -1 || ret == 0)
    return (-1);
  i = -1;
  while (buff[++i])
    {
      client->buff.data[client->buff.write_ptr] = buff[i];
      if (++(client->buff.write_ptr) == RINGLENGTH)
	client->buff.write_ptr = 0;
    }
  return (0);
}

int	ring_in_buff(char *buff, char *str, int pos)
{
  int	i;

  i = -1;
  while (1)
    {
      if (str[pos] == '\r' &&
	  str[((pos + 1 == RINGLENGTH) ? 0 : pos + 1)] == '\n')
	{
	  printf("ok\n");
	  str[pos] = '\0';
	  pos = ((pos + 1 == RINGLENGTH) ? 0 : pos + 1);
	  str[pos] = '\0';
	  return (1);
	}
      buff[++i] = str[pos];
      str[pos] = '\0';
      if (++pos == RINGLENGTH)
	pos = 0;
    }
  buff[++i] = '\0';
  buff[++i] = '\0';
  return (0);
}

int	command_cmp(char *command, char *str, int pos)
{
  int	i;

  i = -1;
  while (command[++i])
    if (command[i] != toupper(str[pos + i]))
      return (0);
  if (str[pos + i] != ' ' && str[pos + i] != '\0')
    return (0);
  return (1);
}

char	*first_arg(char *user)
{
  int	i;

  i = -1;
  while (user[++i] && user[i] != ' ');
  user[i] = '\0';
  return (user);
}

int	connect_client(t_client *client, t_inf *inf)
{
  if (!client->registered && client->nick && client->user
      && client->ping.idle == 0)
    {
      client->registered = 1;
      dprintf(client->fd, "%s 001 %s :Welcome %s!%s@%s\r\n", inf->hostname, client->nick, client->nick, first_arg(client->user), client->hostname);
      dprintf(client->fd, "%s 002 %s :Your host is %s\r\n", inf->hostname, client->nick, inf->hostname);
      dprintf(client->fd, "%s 003 %s :This server was created on %s\r\n", inf->hostname, client->nick, CREATION_DATE);
      dprintf(client->fd, "%s 004 %s :...\r\n", inf->hostname, client->nick);
      dprintf(client->fd, "%s 005 %s :Try server \"none\" instead\r\n", inf->hostname, client->nick);
      dprintf(client->fd, ":%s MODE %s :+iwx\r\n", client->nick, client->nick);
    }
  return (0);
}

int	first_arg_pos(char *arg)
{
  int	tmp;
  int	i;

  i = -1;
  while (arg[++i] == ' ');
  if (arg[i] != ':')
    return (i);
  tmp = i;
  --i;
  while (arg[++i] && arg[i] != ' ');
  if (!arg[i])
    return (tmp);
  while (arg[++i] == ' ');
  if (arg[i] == '\0')
    return (tmp);
  return (i);
}

int	get_arg_pos(char *buff, int nbr)
{
  int	i;
  int	arg;

  arg = 0;
  i = first_arg_pos(buff) - 1;
  while (buff[++i])
    {
      if (buff[i] != ' ' && (i == 0 || buff[i - 1] == ' '))
	++arg;
      if (arg == nbr)
	return (i);
    }
  return (-1);
}

int	is_digitletter(char c)
{
  if ((c >= 'a' && c <= 'z') ||
      (c >= 'A' && c <= 'Z') ||
      (c >= '0' && c <= '9'))
    return (1);
  return (0);
}

int	send_to_client(char *msg, t_client *client)
{
  dprintf(client->fd, ":%s %s\r\n", HOSTNAME, msg);
  return (0);
}

t_client	*find_client_by_name(char *name, t_client *client,
				     t_inf *inf)
{
  t_client	*tmp;

  tmp = inf->client;
  while (tmp)
    {
      if (strcmp(name, tmp->nick) == 0 &&
	  client != tmp)
	return (tmp);
      tmp = tmp->next;
    }
  return (NULL);
}

void	send_ping(t_client *client)
{
  client->ping.timer = time(NULL);
  client->ping.idle = 1;
  dprintf(client->fd, "PING :%s\r\n", HOSTNAME);
}

int	nick_success(t_client *client, t_inf *inf)
{
  printf("Nick ok\n");
  if (client->registered == 0 && client->ping.first == 0)
    {
      printf("Ping\n");
      client->ping.first = 1;
      send_ping(client);
    }
  return (connect_client(client, inf));
}

int	check_nick(t_client *client, t_inf *inf, char *old)
{
  char	specials[] = "-[]\\`^{}";
  int	i;
  char	illegal;
  int	y;

  i = -1;
  illegal = 0;
  if (client->nick[0] == '\0')
    {
      send_to_client("431 No nickname given", client);
      client->nick = old;
      return (0);
    }
  if (find_client_by_name(client->nick, client, inf))
    {
      dprintf(client->fd, ":%s 433 %s %s :Nickname is already in use\r\n",
	      HOSTNAME, (old ? old : "*"), client->nick);
      client->nick = old;
      return (0);
    }
  while (client->nick[++i])
    {
      if (!is_digitletter(client->nick[i]))
	{
	  illegal = 1;
	  y = -1;
	  while (specials[++y])
	    if (specials[y] == client->nick[i])
	      illegal = 0;
	  if (illegal)
	    {
	      client->nick = old;
	      return (send_to_client("432 Illegal characters", client));
	    }
	}
    }
  return (nick_success(client, inf));
}

int	nick_command(t_client *client, t_inf *inf, char *buff)
{
  char	*nick;
  int	i;
  int	pos;
  char	*old;

  if ((pos = get_arg_pos(buff, 2)) == -1)
    return (send_to_client("431 No nickname given", client));
  i = -1;
  while (buff[pos + ++i] && buff[pos + i] != ' ');
  if ((nick = malloc(i + 1)) == NULL)
    return (1);
  i = -1;
  while (buff[pos + ++i] && buff[pos + i] != ' ')
    nick[i] = buff[pos + i];
  nick[i] = '\0';
  old = client->nick;
  client->nick = nick;
  return (check_nick(client, inf, old));
}

void	user_in_data(t_client *client, char *arg)
{
  int	i;
  int	y;

  if ((client->user = malloc(strlen(&arg[get_arg_pos(arg, 2)]) + 1)) == NULL)
    return ;
  i = get_arg_pos(arg, 2) - 1;
  y = -1;
  while (arg[++i])
    client->user[++y] = arg[i];
  client->user[++y] = '\0';
  printf("%s\n", client->user);
}

int	user_command(t_client *client, t_inf *inf, char *arg)
{
  int	i;

  i = -1;
  while (++i < 4)
    if (get_arg_pos(&arg[first_arg_pos(arg)], i + 2) == -1)
      {
	dprintf(client->fd, ":%s 461 %s Not enough parameters\r\n",
		inf->hostname, (client->nick ? client->nick : ""));
	return (0);
      }
  if (client->registered)
    dprintf(client->fd, ":%s 462 %s You may not reregister\r\n",
	    inf->hostname, (client->nick ? client->nick : ""));
  else
    user_in_data(client, &arg[first_arg_pos(arg)]);
  return (connect_client(client, inf));
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
	  inf->hostname, inf->hostname, &arg[pos]);
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

int	bad_command(t_inf *inf, t_client *client)
{
  if (client->registered == 0)
    dprintf(client->fd, ":%s 451 You have not registered\r\n",
	    inf->hostname);
  else
    dprintf(client->fd, ":%s 421 Unknown command\r\n",
	    inf->hostname);
  return (0);
}

t_channel	*find_chan(char *str, t_inf *inf)
{
  t_channel	*tmp;

  tmp = inf->channel;
  while (tmp)
    {
      if (strcmp(tmp->name, str) == 0)
	return (tmp);
      tmp = tmp->next;
    }
  return (NULL);
}

t_channel	*create_chan(char *str, t_inf *inf)
{
  t_channel	*tmp;
  t_channel	*new_chan;

  if ((new_chan = malloc(sizeof(t_channel))) == NULL)
    return (NULL);
  if ((new_chan->name = strdup(str)) == NULL)
    return (NULL);
  new_chan->member = NULL;
  new_chan->next = NULL;
  if (inf->channel == NULL)
    inf->channel = new_chan;
  else
    {
      tmp = inf->channel;
      while (tmp && tmp->next)
	tmp = tmp->next;
      tmp->next = new_chan;
    }
  return (new_chan);
}

int	count_users(t_channel *chan)
{
  int		nbr;
  t_member	*tmp;

  nbr = 0;
  tmp = chan->member;
  while (tmp)
    {
      ++nbr;
      tmp = tmp->next;
    }
  return (nbr);
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

  if (chan_name[0] != '#' && chan_name[0] != '$')
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

void	send_custom_to_chan(t_client *client, t_channel *chan, char *txt)
{
  t_member	*tmp;

  tmp = chan->member;
  while (tmp)
    {
      if (tmp->fd != client->fd)
	dprintf(tmp->fd, "%s\r\n", txt);
      tmp = tmp->next;
    }
}

void	client_read_error(t_client *client)
{
  t_join	*chan;
  char		buff[200];

  sprintf(buff, ":%s!%s@%s QUIT :Read error", client->nick,
	  first_arg(client->user), client->hostname);
  chan = client->chan;
  while (chan)
    {
      send_custom_to_chan(client, chan->chan, buff);
      chan = chan->next;
    }
  chan = client->chan;
  while (chan)
    {
      delete_client_from_chan(client, chan->chan);
      chan = client->chan;
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
  delete_client_from_chan(client, tmp);
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

int	check_command(char *buff, t_inf *inf, t_client *client)
{
  int		i;
  static char	*commands[] =
    {
      "NICK", "USER", "PING", "PONG", "JOIN", "PRIVMSG",
      "PART", 0
    };
  static int	(*fnc[])(t_client *, t_inf *, char *) =
    {
      nick_command, user_command, ping_command, pong_command,
      join_command, privmsg_command, part_command
    };

  printf("%s\n", buff);
  if (buff[0] == '\0')
    return (0);
  i = -1;
  while (commands[++i])
    if (command_cmp(commands[i], buff, first_arg_pos(buff)))
      return (fnc[i](client, inf, buff));
  return (bad_command(inf, client));
}

int	check_ring(t_client *client, t_inf *inf, char first, char prot)
{
  static char	buff[RINGLENGTH + 1];
  int		tmp;

  bzero(buff, RINGLENGTH);
  tmp = client->buff.read_ptr;
  while ((first == 0 || (client->buff.read_ptr != tmp)) &&
	  client->buff.data[client->buff.read_ptr] != '\0')
    {
      first = 1;
      if (client->buff.read_ptr == RINGLENGTH)
	client->buff.read_ptr = 0;
      if (client->buff.data[client->buff.read_ptr] == '\r')
	prot = 1;
      else if (client->buff.data[client->buff.read_ptr] == '\n'
	       && prot == 1)
	{
	  printf("ah\n");
	  ++client->buff.read_ptr;
	  ring_in_buff(buff, client->buff.data, tmp);
	  return (check_command(buff, inf, client));
	}
      else
	prot = 0;
      ++client->buff.read_ptr;
    }
  return (1);
}

void	free_client(t_client *client)
{
  if (client->nick)
    free(client->nick);
  if (client->user)
    free(client->user);
  free(client);
}

void	delete_client(int fd, t_inf *inf)
{
  t_client	*previous;
  t_client	*tmp;

  previous = NULL;
  tmp = inf->client;
  while (tmp)
    {
      if (tmp->fd == fd)
	{
	  close(fd);
	  if (previous)
	    previous->next = tmp->next;
	  else
	    inf->client = tmp->next;
	  free_client(tmp);
	  return ;
	}
      previous = tmp;
      tmp = tmp->next;
    }
}

int	read_client(int client_fd, fd_set *set, t_inf *inf)
{
  t_client	*client;

  if ((client = get_client(client_fd, inf)) == NULL)
    return (-1);
  if (read_socket(client_fd, client) == -1)
    {
      client_read_error(client);
      delete_client(client_fd, inf);
      FD_CLR(client_fd, set);
      return (0);
    }
  else
    while (check_ring(client, inf, 0, 0) == 0);
  return (0);
}

int	check_signal(int sfd)
{
  struct signalfd_siginfo	si;

  read(sfd, &si, sizeof(si));
  if (si.ssi_signo == SIGINT || si.ssi_signo == SIGTERM)
    {
      printf("Signal caught, closing.\n");
      return (1);
    }
  return (0);
}

int	check_set(fd_set *try, t_inf *inf, fd_set *set)
{
  int	i = -1;

  if (FD_ISSET(inf->signal, try))
    return (check_signal(inf->signal));
  else if (FD_ISSET(inf->server, try))
    accept_new_client(set, inf);
  else
    {
      while (++i < FD_SETSIZE)
	{
	  if (FD_ISSET(i, try))
	    read_client(i, set, inf);
	}
    }
  return (0);
}

int	init_signals()
{
  int		sfd;
  sigset_t	mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0 ||
      (sfd = signalfd(-1, &mask, 0)) < 0)
    return (-1);
  return (sfd);
}

int	client_timer(t_client *client, int timesec, t_inf *inf, fd_set *set)
{
  if (client->registered == 0 && timesec >= LOG_TIMEOUT_SEC)
    {
      FD_CLR(client->fd, set);
      dprintf(client->fd, "ERROR :Closing Link (Registration Timeout:");
      dprintf(client->fd, " %d seconds)\r\n", LOG_TIMEOUT_SEC);
      delete_client(client->fd, inf);
      return (1);
    }
  if (client->ping.idle == 0 && timesec >= PING_DELAY)
    {
      client->ping.idle = 1;
      client->ping.timer = time(NULL);
      dprintf(client->fd, ":%s PING :%s\r\n", inf->hostname, inf->hostname);
    }
  else if (client->ping.idle && timesec >= IDLE_TIMEOUT_SEC)
    {
      FD_CLR(client->fd, set);
      dprintf(client->fd, "ERROR :Closing Link (Ping Timeout:");
      dprintf(client->fd, " %d seconds)\r\n", IDLE_TIMEOUT_SEC);
      delete_client(client->fd, inf);
      return (1);
    }
  return (0);
}

int	update_timers(t_inf *inf, fd_set *set)
{
  t_client	*tmp;

  tmp = inf->client;
  while (tmp)
    {
      if (client_timer(tmp, time(NULL) - tmp->ping.timer, inf, set))
	tmp = inf->client;
      else
	tmp = tmp->next;
    }
  return (0);
}

int	server_loop(t_inf *inf)
{
  fd_set		set;
  fd_set		try;
  int			ret;
  struct timeval	timerange;

  FD_ZERO(&set);
  FD_SET(inf->server, &set);
  FD_SET(inf->signal, &set);
  timerange.tv_sec = 0;
  timerange.tv_usec = 0;
  while (1)
    {
      try = set;
      if ((ret = select(FD_SETSIZE, &try, NULL, NULL, &timerange)) == -1)
	return (1);
      if (ret > 0)
	if (check_set(&try, inf, &set))
	  return (0);
      update_timers(inf, &set);
      usleep(100);
    }
  return (0);
}

void	delete_clients(t_inf *inf)
{
  while (inf->client)
    delete_client(inf->client->fd, inf);
  close(inf->signal);
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
