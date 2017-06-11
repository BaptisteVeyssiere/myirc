/*
** welcome.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 17:46:17 2017 Nathan Scutari
** Last update Sun Jun 11 17:47:58 2017 Nathan Scutari
*/

#include "server.h"

int	connect_client(t_client *client, t_inf *inf)
{
  if (!client->registered && client->nick && client->user
      && client->ping.idle == 0)
    {
      client->registered = 1;
      dprintf(client->fd, ":%s 001 %s :Welcome %s!%s@%s\r\n", inf->hostname, client->nick,
	      client->nick, first_arg(client->user), client->hostname);
      dprintf(client->fd, ":%s 002 %s :Your host is %s\r\n", inf->hostname, client->nick, inf->hostname);
      dprintf(client->fd, ":%s 003 %s :This server was created on %s\r\n", inf->hostname, client->nick, CREATION_DATE);
      dprintf(client->fd, ":%s 004 %s :...\r\n", inf->hostname, client->nick);
      dprintf(client->fd, ":%s 005 %s :Try server \"none\" instead\r\n", inf->hostname, client->nick);
      dprintf(client->fd, ":%s MODE %s :+iwx\r\n", client->nick, client->nick);
    }
  return (0);
}
