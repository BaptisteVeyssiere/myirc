/*
** server.h for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/include
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 14:18:03 2017 Nathan Scutari
** Last update Sun Jun 11 14:32:47 2017 Nathan Scutari
*/

#ifndef _SERVER_H_
# define SERVER_H_

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
#include "ringbuffer.h"

#define CLIENT_HOST_SIZE	60
#define CREATION_DATE		"03/06/2017"
#define HOSTNAME		"irc.server.tek"
#define LOG_TIMEOUT_SEC		45
#define PING_DELAY		600
#define IDLE_TIMEOUT_SEC	120
#define UNUSED			__attribute__((unused))
#define RINGLENGTH		4096

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

typedef struct		s_file
{
  struct s_client	*send;
  struct s_client	*get;
}			t_file;

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
  t_file		request;
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
  fd_set		*set;
  char			*hostname;
  t_client		*client;
  t_channel		*channel;
}			t_inf;

/*
** init_server.c
*/
int	get_server_socket(char *);
int	init_signals();

/*
** accept_client.c
*/
int	accept_new_client(fd_set *, t_inf *);
void	init_client(t_client *);
int	add_client(int, t_inf *);
int	get_clienthostname(t_client *, struct sockaddr_in *,
			   socklen_t);

#endif /* !SERVER_H_ */
