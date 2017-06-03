/*
** main.c for irc in /home/scutar_n/rendu/PSU/myirc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Tue May 30 11:21:20 2017 Nathan Scutari
** Last update Sat Jun  3 12:12:48 2017 Nathan Scutari
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

typedef struct		s_client
{
  char			*user;
  char			*nick;
  int			fd;
  t_ringbuffer		buff;
  t_ping		ping;
  int			registered;
  struct s_client	*next;
}			t_client;

typedef struct		s_channel
{
  char			*name;
  int			*fd;
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

int	not_registered(t_client *client)
{
  if (!client->user || !client->nick)
    return (1);
  return (0);
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

int	accept_new_client(fd_set *set, t_inf *inf)
{
  int	client_fd;

  if ((client_fd = accept(inf->server, NULL, NULL)) == -1)
    return (print_err("accept failed\n", -1));
  if ((add_client(client_fd, inf)) == -1)
    return (-1);
  FD_SET(client_fd, set);
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
	return (1);
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

int	connect_client(t_client *client, t_inf *inf)
{
  if (!client->registered && client->nick && client->user)
    {
      client->registered = 1;
      dprintf(client->fd, "%s 001 :Welcome %s\r\n", inf->hostname, client->nick);
      dprintf(client->fd, "%s 002 :Your host is %s\r\n", inf->hostname, inf->hostname);
      dprintf(client->fd, "%s 003 :This server was created on %s\r\n", inf->hostname, CREATION_DATE);
      dprintf(client->fd, "%s 004 :...\r\n", inf->hostname);
      dprintf(client->fd, "%s 005 :Try server \"none\" instead\r\n", inf->hostname);
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

void	send_ping(t_client *client, t_inf *inf)
{
  client->ping.timer = time(NULL);
  client->ping.idle = 1;
  dprintf(client->fd, ":%s PING %s\r\n", inf->hostname, inf->hostname);
}

int	nick_success(t_client *client, t_inf *inf)
{
  if (not_registered(client) && client->ping.first == 0)
    {
      client->ping.first = 1;
      send_ping(client, inf);
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
      client->nick = old;
      return (send_to_client("431 No nickname given", client));
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

int	check_command(char *buff, t_inf *inf, t_client *client)
{
  int		i;
  static char	*commands[] =

    {
      "NICK", "USER", "PING", "PONG", 0
    };
  static int	(*fnc[])(t_client *, t_inf *, char *) =
    {
      nick_command, user_command, ping_command, pong_command
    };

  printf("%s\n", buff);
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
  while (first == 0 || client->buff.read_ptr != tmp)
    {
      first = 1;
      if (client->buff.read_ptr == RINGLENGTH)
	client->buff.read_ptr = 0;
      if (client->buff.data[client->buff.read_ptr] == '\r')
	prot = 1;
      else if (client->buff.data[client->buff.read_ptr] == '\n'
	       && prot == 1)
	{
	  ++client->buff.read_ptr;
	  ring_in_buff(buff, client->buff.data, tmp);
	  return (check_command(buff, inf, client));
	}
      else
	prot = 0;
      ++client->buff.read_ptr;
    }
  return (0);
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
      delete_client(client_fd, inf);
      FD_CLR(client_fd, set);
      return (0);
    }
  else
    return (check_ring(client, inf, 0, 0));
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
  if (not_registered(client) && timesec >= LOG_TIMEOUT_SEC)
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
  close(inf.server);
  return (ret);
}
