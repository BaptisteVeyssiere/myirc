/*
** server.h for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/include
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 14:18:03 2017 Nathan Scutari
** Last update Sun Jun 11 15:19:19 2017 Nathan Scutari
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

/*
** loop.c
*/
int	server_loop(t_inf *);
int	check_set(fd_set *, t_inf *, fd_set *);
int	check_signal(int);

/*
** ringbuffer.c
*/
int	ring_in_buff(char *, t_client *, int, t_inf *);
int	check_ring(t_client *, t_inf *, char, char);
int	read_client(int, fd_set *, t_inf *);
int	read_socket(int, t_client *);

/*
** command_check.c
*/
int	check_command(char *, t_inf *, t_client *);
int	command_cmp(char *, char *, int);
int	bad_command(t_inf *, t_client *);

/*
** timeout.c
*/
int	update_timers(t_inf *, fd_set *);
int	client_timer(t_client *, int, t_inf *, fd_set *);

/*
** channel_tools.c
*/
int		count_users(t_channel *);
t_channel	*find_chan(char *, t_inf *);
void		send_custom_to_chan(t_client *, t_channel *, char *);
void		remove_chan(t_channel *, t_inf *);
t_channel	*create_chan(char *, t_inf *);

/*
** free.c
*/
void	delete_chan_from_client(t_client *, t_channel *);
void	delete_client_from_chan(t_client *, t_channel *, t_inf *);
void	free_client(t_client *);
void	delete_client(int, t_inf *);
void	delete_clients(t_inf *);

/*
** client_tools.c
*/
t_client	*get_client(int, t_inf *);
t_client	*find_client_by_name(char *, t_client *, t_inf *);
int		in_same_channel(t_client *, t_client *);
void		client_read_error(t_client *, t_inf *);
void		disconnect_client(t_client *, char *, int, t_inf *);

#endif /* !SERVER_H_ */
