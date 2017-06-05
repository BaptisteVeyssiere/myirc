/*
** client.h for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 13:43:09 2017 Baptiste Veyssiere
** Last update Mon Jun  5 17:03:56 2017 Baptiste Veyssiere
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

# include "ringbuffer.h"
# include "get_next_line.h"

# define CONNECTION_ON "Connection with server established\n"
# define ALREADY_CONNECT "Already connected to a server\n"
# define SIGNAL_CAUGHT "Signal caught, closing session...\n"
# define UNUSED __attribute((unused))
# define MAX_LEN 256

typedef struct	s_client
{
  char		server_on;
  char		*server_name;
  char		*nickname;
  int		fd;
  t_ringbuffer	ringbuffer;
  char		*response;
  char		*user_input;
  char		user_on;
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
** ringbuffer_manager.c
*/

int	get_response(t_ringbuffer *ringbuffer, int server_fd);

/*
** extract_response.c
*/

int	extract_response(t_ringbuffer *ringbuffer, char **response);

#endif /* !CLIENT_H_ */
