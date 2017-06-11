/*
** server.h for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/include
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 14:18:03 2017 Nathan Scutari
** Last update Sun Jun 11 18:11:16 2017 Nathan Scutari
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
void	init_client(t_client *, int);
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

/*
** user.c
*/
int	user_command(t_client *, t_inf *, char *);
void	user_in_data(t_client *, char *);

/*
** arg_tools.c
*/
char	*first_arg(char *);
int	first_arg_pos(char *);
int	get_arg_pos(char *, int);

/*
** nick.c
*/
int	nick_command(t_client *, t_inf *, char *);
int	nick_err_check(t_client *, char *, t_inf *);
void	inform_nick(t_client *, char *, t_inf *);
int	check_nick(t_client *, t_inf *, char *);
int	nick_success(t_client *, t_inf *);

/*
** nick_tools.c
*/
int	is_digitletter(char);
int	is_special(char);

/*
** ping.c
*/
int	ping_command(t_client *, t_inf *, char *);
int	err_registered(t_client *, char *);

/*
** pong.c
*/
int	pong_command(t_client *, t_inf *, char *);

/*
** join.c
*/
int	join_command(t_client *, t_inf *, char *);
int	join_chan(t_inf *, t_client *, char *);
void	inform_all_join(t_channel *, t_client *);

/*
** privmsg.c
*/
int	privmsg_arg_check(int *, int *, char **, t_client *);
int	privmsg_command(t_client *, t_inf *, char *);
int	sendprivmsg(t_client *, char *, char *, t_inf *);
int	send_to_chan(t_client *, t_channel *, char *);
int	send_private(t_client *, t_client *, char *);

/*
** part.c
*/
int	part_command(t_client *, t_inf *, char *);
void	leave_chan(t_client *, char *, int, t_inf *);

/*
** names.c
*/
void	name_chan(char *, t_client *, t_inf *);
int	names_command(t_client *, t_inf *, char *);
void	inform_client_join(t_channel *, t_client *, t_inf *);

/*
** quit.c
*/
void	inform_quit(t_client *, t_client *, char *, int);
int	quit_command(t_client *, t_inf *, char *);

/*
** list.c
*/
int	list_command(t_client *, t_inf *, char *);

/*
** users.c
*/
int	users_command(t_client *, t_inf *, char *);

/*
** file.c
*/
int	get_file(t_client *);
int	refuse_file(t_client *);
int	file_check_arg(char *, int, t_client *, t_inf *);
int	file_command(t_client *, t_inf *, char *);

/*
** file_tools.c
*/
int		send_file(t_client *, t_inf *, char *);
t_client	*check_errors(char *, int , t_client *, t_inf *);
int		check_file_arg_nbr(char *, t_client *, int *);

/*
** tools.c
*/
int	print_err(char *, int);
int	is_ipaddress(char *);
int	send_to_client(char *, t_client *);
void	send_ping(t_client *);
void	cancel_requests(t_client *);

/*
** client_chan_tools.c
*/
int	add_chan_to_client(t_channel *, t_client *);
int	add_client_to_chan(t_client *, t_channel *, int);

/*
** welcome.c
*/
int	connect_client(t_client *, t_inf *);

#endif /* !SERVER_H_ */
