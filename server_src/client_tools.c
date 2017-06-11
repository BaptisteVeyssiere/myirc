/*
** client_tools.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 15:14:25 2017 Nathan Scutari
** Last update Sun Jun 11 15:19:18 2017 Nathan Scutari
*/

#include "server.h"

t_client	*get_client(int fd, t_inf *inf)
{
  t_client	*tmp;

  tmp = inf->client;
  while (tmp)
    {
      if (tmp->fd == fd)
	return (tmp);
      tmp = tmp->next;
    }
  return (NULL);
}

t_client	*find_client_by_name(char *name, t_client *client,
				     t_inf *inf)
{
  t_client	*tmp;

  tmp = inf->client;
  while (tmp)
    {
      if (tmp->nick && strcmp(name, tmp->nick) == 0
	  && client != tmp)
	return (tmp);
      tmp = tmp->next;
    }
  return (NULL);
}

int	in_same_channel(t_client *client, t_client *ref)
{
  t_join	*j_client;
  t_join	*j_ref;

  j_client = client->chan;
  while (j_client)
    {
      j_ref = ref->chan;
      while (j_ref)
	{
	  if (j_ref->chan == j_client->chan)
	    return (1);
	  j_ref = j_ref->next;
	}
      j_client = j_client->next;
    }
  return (0);
}

void	client_read_error(t_client *client, t_inf *inf)
{
  t_join	*chan;
  char		buff[200];

  sprintf(buff, ":%s!%s@%s QUIT :Read error", client->nick,
	  (client->user ? first_arg(client->user) : ""),
	  client->hostname);
  chan = client->chan;
  while (chan)
    {
      send_custom_to_chan(client, chan->chan, buff);
      chan = chan->next;
    }
  chan = client->chan;
  while (chan)
    {
      delete_client_from_chan(client, chan->chan, inf);
      chan = client->chan;
    }
}

void	disconnect_client(t_client *client, char *str, int msg, t_inf *inf)
{
  t_join	*join;

  dprintf(client->fd, "ERROR :Closing Link: %s[%s] (Quit: %s)\r\n",
	  client->nick, client->hostname,
	  (msg == -1 ? client->nick : &str[msg]));
  join = client->chan;
  while (join)
    {
      delete_client_from_chan(client, join->chan, inf);
      join = client->chan;
    }
  FD_CLR(client->fd, inf->set);
  delete_client(client->fd, inf);
}
