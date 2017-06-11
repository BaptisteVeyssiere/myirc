/*
** tools.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 17:28:36 2017 Nathan Scutari
** Last update Sun Jun 11 17:30:29 2017 Nathan Scutari
*/

#include "server.h"

int	print_err(char *msg, int ret)
{
  write(2, msg, strlen(msg));
  return (ret);
}

int	is_ipaddress(char *str)
{
  int	i;

  i = -1;
  while (str[++i])
    {
      if ((str[i] < '0' || str[i] > '9') &&
	  str[i] != '.' && str[i] != ',')
	return (0);
    }
  return (1);
}

int	send_to_client(char *msg, t_client *client)
{
  dprintf(client->fd, ":%s %s\r\n", HOSTNAME, msg);
  return (0);
}

void	send_ping(t_client *client)
{
  client->ping.timer = time(NULL);
  client->ping.idle = 1;
  dprintf(client->fd, "PING :%s\r\n", HOSTNAME);
}

void	cancel_requests(t_client *tmp)
{
  if (tmp->request.send)
    {
      tmp->request.send->request.get = NULL;
      tmp->request.send = NULL;
    }
  if (tmp->request.get)
    {
      tmp->request.get->request.send = NULL;
      tmp->request.get = NULL;
    }
}
