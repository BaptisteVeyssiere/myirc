/*
** client.h for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 13:43:09 2017 Baptiste Veyssiere
** Last update Sun Jun 11 17:39:33 2017 Baptiste Veyssiere
*/

#ifndef CLIENT_H_
# define CLIENT_H_

# include <string.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <signal.h>
# include <sys/signalfd.h>
# include <errno.h>
# include <stdio.h>
# include <sys/select.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "ringbuffer.h"
# include "get_next_line.h"

# define CONNECTION_ON "Connection with server established\n"
# define ALREADY_CONNECT "Already connected to a server\n"
# define SIGNAL_CAUGHT "Signal caught, closing session...\n"
# define INVALID_PARAM "Invalid number of parameters\n"
# define INVALID_CHAN "Invalid channel name\n"
# define USER "user unknown unknown unknown unknown\r\n"
# define NAMES_PROMPT "Users in the channel:\n"
# define CONNECTION_LOST "Connection with server lost\n"
# define UNUSED __attribute((unused))
# define MAX_LEN 256
# define READING_SIZE 256

typedef struct	s_file_trans
{
  int		socket_fd;
  char		*name;
  char		*ip;
  char		*friend;
}		t_file_trans;

typedef struct	s_client
{
  char		server_on;
  char		*server_name;
  char		*nickname;
  int		fd;
  t_ringbuffer	buff;
  char		*user_input;
  char		user_on;
  char		*channel_name;
  char		waiting_nick;
  char		waiting_channel;
  char		waiting_names;
  char		waiting_users;
  char		waiting_list;
  char		*list_filter;
  char		first_response;
  char		*username;
  t_file_trans	file;
}		t_client;

/*
** client.c
*/

int	client(int signal_fd);

/*
** server.c
*/

int	server(const char **tab, const char *src, t_client *client);

/*
** epur_str.c
*/

char	*epur_str(const char *s);

/*
** strtab.c
*/

char	**strtab(const char *s);

/*
** check_ip_port.c
*/

char	ip_isvalid(const char *ip);
int	port_isvalid(const char *port);

/*
** signal_handler.c
*/

int	check_signal(int fd);
int	init_signals(void);

/*
** check_server_response.c
*/

int	check_server_response(t_client *client, int signal_fd);

/*
** write_error.c
*/

int	write_error(const char *func, const char *file, int line);
int	free_server(char *ip, char *port);

/*
** nick.c
*/

int	nick(const char **tab, const char *src, t_client *client);

/*
** check_command.c
*/

int	check_command(const char *command, t_client *client);

/*
** join.c
*/

int	join(const char **tab, const char *src, t_client *client);

/*
** part.c
*/

int	part(const char **tab, const char *src, t_client *client);

/*
** names.c
*/

int	names(const char **tab, const char *src, t_client *client);

/*
** message.c
*/

int	message(const char **tab, const char *src, t_client *client);

/*
** channel_msg.c
*/

int	send_msg_to_channel(const char *src, t_client *client);

/*
** table_manager.c
*/

int	check_param_nbr(const char **tab);

/*
** user_msg.c
*/

int	send_msg_to_user(const char **tab, const char *src, t_client *client);

/*
** check_waiting.c
*/

int	check_nick(const char *command, t_client *client);

/*
** pong.c
*/

int	pong(const char *command, t_client *client);

/*
** check_join.c
*/

int	check_join(const char *command, t_client *client);

/*
** check_names.c
*/

int	check_names(const char *command, t_client *client);

/*
** check_part.c
*/

int	check_part(const char *command, t_client *client);

/*
** message_response.c
*/

int	message_response(const char *command);

/*
** quit.c
*/

int	quit(const char **tab, const char *src, t_client *client);

/*
** users.c
*/

int	check_users(const char *command);
int	check_first_user(const char *command, t_client *client);
int	check_last_user(const char *command, t_client *client);
int	users(const char **tab, const char *src, t_client *client);

/*
** list.c
*/

int	check_list(const char *command, t_client *client);
int	check_first_list(const char *command, t_client *client);
int	check_last_list(const char *command, t_client *client);
int	list(const char **tab, const char *src, t_client *client);

/*
** refuse_file.c
*/

int	refuse_file(const char **tab, const char *src, t_client *client);

/*
** accept_file.c
*/

int	accept_file(const char **tab, const char *src, t_client *client);

/*
** send_file.c
*/

int	send_file(const char **tab, t_client *client);

/*
** check_refuse.c
*/

int	check_refuse(const char *command, t_client *client);

/*
** check_accept.c
*/

int	check_accept(const char *command, t_client *client);

/*
** check_send.c
*/

int	check_send(const char *command, t_client *client);

/*
** check_quit.c
*/

int	check_quit(const char *command);

/*
** ident_command.c
*/

int	ident_command(const char *line, t_client *client, const char *src);

#endif /* !CLIENT_H_ */
