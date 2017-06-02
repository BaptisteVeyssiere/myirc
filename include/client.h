/*
** client.h for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 13:43:09 2017 Baptiste Veyssiere
** Last update Fri Jun  2 10:39:13 2017 Baptiste Veyssiere
*/

#ifndef CLIENT_H_
# define CLIENT_H_

# include <string.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>

# include "ringbuffer.h"
# include "get_next_line.h"

# define CONNECTION_ON "Connection with server established\n"
# define ALREADY_CONNECT "Already connected to a server\n"
# define UNUSED __attribute((unused))

typedef struct	s_client
{
  char		server_on;
  char		*server_name;
  char		*nickname;
  int		fd;
}		t_client;

/*
** client.c
*/

int	client(void);

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

#endif /* !CLIENT_H_ */
