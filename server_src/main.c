/*
** main.c for irc in /home/scutar_n/rendu/PSU/myirc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Tue May 30 11:21:20 2017 Nathan Scutari
** Last update Thu Jun  1 13:33:47 2017 Nathan Scutari
*/

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

#define RINGLENGTH	150

typedef struct		s_ringBuffer
{
  char			data[RINGLENGTH];
  int			writePtr;
  int			readPtr;
}			t_ringBuffer;

typedef struct		s_client
{
  char			*user;
  char			*nick;
  int			fd;
  t_ringBuffer		buff;
  int			timeout;
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
  t_client		*client;
  t_channel		*channel;
}			t_inf;

int	printErr(char *msg, int ret)
{
  write(2, msg, strlen(msg));
  write(2, "\n", 1);
  return (ret);
}

int	getServerSocket(char *arg)
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
    return (printErr("getprotobybyname failed\n", -1));
  if ((fd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    return (printErr("socket failed\n", -1));
  if (bind(fd, (const struct sockaddr *)&s_in, sizeof(s_in)) == -1)
    {
      close(fd);
      return (printErr("bind failed\n", -1));
    }
  return (fd);
}

int	addClient(int client_fd, t_inf *inf)
{
  t_client	*tmp;
  t_client	*new_client;

  if ((new_client = malloc(sizeof(t_client))) == NULL)
    return (printErr("malloc failed\n", -1));
  new_client->user = NULL;
  new_client->nick = NULL;
  new_client->fd = client_fd;
  new_client->buff.writePtr = 0;
  new_client->buff.readPtr = 0;
  new_client->timeout = 0;
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

int	acceptNewClient(fd_set *set, t_inf *inf)
{
  int	client_fd;

  if ((client_fd = accept(inf->server, NULL, NULL)) == -1)
    return (printErr("accept failed\n", -1));
  if ((addClient(client_fd, inf)) == -1)
    return (-1);
  FD_SET(client_fd, set);
  return (0);
}

t_client	*getClient(int fd, t_inf *inf)
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

int	readSocket(int fd, t_client *client)
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
      client->buff.data[client->buff.writePtr] = buff[i];
      if (++(client->buff.writePtr) == RINGLENGTH)
	client->buff.writePtr = 0;
    }
  return (0);
}

int	readClient(int client_fd, fd_set *set, t_inf *inf)
{
  t_client	*client;

  if ((client = getClient(client_fd, inf)) == NULL)
    return (-1);
  if (readSocket(client_fd, client) == -1)
    {
      close(client_fd);
      FD_CLR(client_fd, set);
    }
  else
    {

    }
  return (0);
}

int	checkSignal(int sfd)
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

int	checkSet(fd_set *try, t_inf *inf, fd_set *set)
{
  int	i = -1;

  if (FD_ISSET(inf->signal, try))
    return (checkSignal(inf->signal));
  else if (FD_ISSET(inf->server, try))
    acceptNewClient(set, inf);
  else
    {
      while (++i < FD_SETSIZE)
	{
	  if (FD_ISSET(i, try))
	    readClient(i, set, inf);
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

int	serverLoop(t_inf *inf)
{
  fd_set		set;
  fd_set		try;
  int			ret;
  struct timeval	time;

  FD_ZERO(&set);
  FD_SET(inf->server, &set);
  FD_SET(inf->signal, &set);
  time.tv_sec = 0;
  time.tv_usec = 0;
  while (1)
    {
      try = set;
      if ((ret = select(FD_SETSIZE, &try, NULL, NULL, &time)) == -1)
	return (1);
      if (ret > 0)
	if (checkSet(&try, inf, &set))
	  return (0);
      usleep(100);
    }
  return (0);
}

int	main(int ac, char **av)
{
  int	ret;
  t_inf	inf;

  if (ac != 2)
    return (printErr("Usage ./server [port]", 1));
  if ((inf.server = getServerSocket(av[1])) == -1 || (inf.signal = init_signals()) == -1)
    return (1);
  if (listen(inf.server, 42) == -1)
    return (printErr("listen failed\n", 1));
  inf.client = NULL;
  inf.channel = NULL;
  ret = serverLoop(&inf);
  close(inf.server);
  return (ret);
}
