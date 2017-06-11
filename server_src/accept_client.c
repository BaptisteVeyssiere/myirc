/*
** accept_client.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 14:28:25 2017 Nathan Scutari
** Last update Sun Jun 11 18:44:58 2017 Nathan Scutari
*/

#include "server.h"

int	accept_new_client(fd_set *set, t_inf *inf)
{
  t_client		*client;
  int			client_fd;
  socklen_t		size;
  struct sockaddr_in	s_in;

  size = sizeof(s_in);
  if ((client_fd = accept(inf->server, (struct sockaddr*)&s_in,
			  &size)) == -1)
    return (print_err("accept failed\n", -1));
  if ((add_client(client_fd, inf)) == -1)
    return (-1);
  if ((client = get_client(client_fd, inf)) == NULL ||
      get_clienthostname(client, &s_in, size) == -1)
    return (-1);
  FD_SET(client_fd, set);
  return (0);
}

void	init_client(t_client *client, int fd)
{
  client->user = NULL;
  client->nick = NULL;
  client->request.get = NULL;
  client->request.send = NULL;
  client->fd = fd;
  client->buff.write_ptr = 0;
  client->buff.read_ptr = 0;
  client->ping.timer = time(NULL);
  client->ping.idle = 0;
  client->ping.first = 0;
  client->next = NULL;
  client->registered = 0;
  client->hostname = NULL;
  client->chan = NULL;
  bzero(client->buff.data, RINGLENGTH);
}

int	add_client(int client_fd, t_inf *inf)
{
  t_client	*tmp;
  t_client	*new_client;

  if ((new_client = malloc(sizeof(t_client))) == NULL)
    return (print_err("Malloc failed\n", -1));
  init_client(new_client, client_fd);
  if (!(inf->client))
    inf->client = new_client;
  else
    {
      tmp = inf->client;
      while (tmp && tmp->next)
	tmp = tmp->next;
      tmp->next = new_client;
    }
  return (0);
}

int	get_clienthostname(t_client *client, struct sockaddr_in *s_in,
			   socklen_t size)
{
  if ((client->hostname = malloc(CLIENT_HOST_SIZE)) == NULL)
    return (-1);
  dprintf(client->fd, ":%s NOTICE * :*** Looking up your hostname...\r\n",
	  HOSTNAME);
  if ((getnameinfo((struct sockaddr*)s_in,
		   size, client->hostname, CLIENT_HOST_SIZE,
		   NULL, 0, NI_NOFQDN)) != 0)
    return (-1);
  if (is_ipaddress(client->hostname))
    {
      dprintf(client->fd, ":%s NOTICE * :*** Could not find your hostname",
	      HOSTNAME);
      dprintf(client->fd, "using your ip address instead (%s)\r\n",
	      client->hostname);
    }
  else
    dprintf(client->fd, ":%s NOTICE * :*** Found your hostname (%s)\r\n",
	    HOSTNAME, client->hostname);
  return (0);
}
