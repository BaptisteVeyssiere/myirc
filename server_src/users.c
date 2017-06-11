/*
** users.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:54:46 2017 Nathan Scutari
** Last update Sun Jun 11 16:56:01 2017 Nathan Scutari
*/

#include "server.h"

int	users_command(t_client *client, t_inf *inf, UNUSED char *arg)
{
  t_client	*tmp;

  if (client->registered == 0)
    return (err_registered(client, "USERS"));
  dprintf(client->fd, ":%s 392 %s Users :UserId Host\r\n",
	  HOSTNAME, client->nick);
  tmp = inf->client;
  while (tmp)
    {
      if (tmp->nick)
	dprintf(client->fd, ":%s 393 %s %s :%s %s\r\n", HOSTNAME,
		client->nick, tmp->nick, first_arg(tmp->user),
		tmp->hostname);
      tmp = tmp->next;
    }
  dprintf(client->fd, ":%s 394 %s :End of /USERS\r\n",
	  HOSTNAME, client->nick);
  return (0);
}
