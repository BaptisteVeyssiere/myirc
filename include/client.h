/*
** client.h for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 13:43:09 2017 Baptiste Veyssiere
** Last update Thu Jun  1 23:33:57 2017 Baptiste Veyssiere
*/

#ifndef CLIENT_H_
# define CLIENT_H_

# include <string.h>

# include "ringbuffer.h"
# include "get_next_line.h"

typedef struct	s_client
{
  char		server_on;
  char		*server_name;
  char		*nickname;
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

#endif /* !CLIENT_H_ */
