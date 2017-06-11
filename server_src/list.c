/*
** list.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:50:59 2017 Nathan Scutari
** Last update Sun Jun 11 16:52:43 2017 Nathan Scutari
*/

#include "server.h"

int	list_command(t_client *client, t_inf *inf, UNUSED char *arg)
{
  t_channel	*tmp;

  if (client->registered == 0)
    return (err_registered(client, "LIST"));
  dprintf(client->fd, ":%s 321 %s Channel :Users Name\r\n",
	  HOSTNAME, client->nick);
  tmp = inf->channel;
  while (tmp)
    {
      dprintf(client->fd, ":%s 322 %s %s %d :\r\n", HOSTNAME,
	      client->nick, tmp->name, count_users(tmp));
      tmp = tmp->next;
    }
  dprintf(client->fd, ":%s 323 %s :End of /LIST\r\n", HOSTNAME,
	  client->nick);
  return (0);
}
