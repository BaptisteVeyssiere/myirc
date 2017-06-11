/*
** timeout.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 15:00:00 2017 Nathan Scutari
** Last update Sun Jun 11 15:02:55 2017 Nathan Scutari
*/

#include "server.h"

int	update_timers(t_inf *inf, fd_set *set)
{
  t_client	*tmp;

  tmp = inf->client;
  while (tmp)
    {
      if (client_timer(tmp, time(NULL) - tmp->ping.timer, inf, set))
	tmp = inf->client;
      else
	tmp = tmp->next;
    }
  return (0);
}

int	client_timer(t_client *client, int timesec, t_inf *inf, fd_set *set)
{
  if (client->registered == 0 && timesec >= LOG_TIMEOUT_SEC)
    {
      FD_CLR(client->fd, set);
      dprintf(client->fd, "ERROR :Closing Link (Registration Timeout:");
      dprintf(client->fd, " %d seconds)\r\n", LOG_TIMEOUT_SEC);
      delete_client(client->fd, inf);
      return (1);
    }
  if (client->ping.idle == 0 && timesec >= PING_DELAY)
    {
      client->ping.idle = 1;
      client->ping.timer = time(NULL);
      dprintf(client->fd, ":%s PING :%s\r\n", inf->hostname, inf->hostname);
    }
  else if (client->ping.idle && timesec >= IDLE_TIMEOUT_SEC)
    {
      FD_CLR(client->fd, set);
      dprintf(client->fd, "ERROR :Closing Link (Ping Timeout:");
      dprintf(client->fd, " %d seconds)\r\n", IDLE_TIMEOUT_SEC);
      delete_client(client->fd, inf);
      return (1);
    }
  return (0);
}
