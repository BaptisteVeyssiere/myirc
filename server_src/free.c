/*
** free.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 15:11:09 2017 Nathan Scutari
** Last update Sun Jun 11 15:14:07 2017 Nathan Scutari
*/

#include "server.h"

void	delete_chan_from_client(t_client *client, t_channel *chan)
{
  t_join	*tmp;
  t_join	*prev;

  tmp = client->chan;
  prev = NULL;
  while (tmp)
    {
      if (tmp->chan == chan)
	{
	  if (prev == NULL)
	    client->chan = tmp->next;
	  else
	    prev->next = tmp->next;
	  free(tmp);
	  return ;
	}
      prev = tmp;
      tmp = tmp->next;
    }
}

void	delete_client_from_chan(t_client *client, t_channel *chan, t_inf *inf)
{
  t_member	*tmp;
  t_member	*prev;

  tmp = chan->member;
  prev = NULL;
  while (tmp)
    {
      if (tmp->fd == client->fd)
	{
	  if (prev == NULL)
	    chan->member = tmp->next;
	  else
	    prev->next = tmp->next;
	  free(tmp);
	  delete_chan_from_client(client, chan);
	  if (count_users(chan) == 0)
	    remove_chan(chan, inf);
	  return ;
	}
      prev = tmp;
      tmp = tmp->next;
    }
}

void	free_client(t_client *client)
{
  if (client->nick)
    free(client->nick);
  if (client->user)
    free(client->user);
  free(client);
}

void	delete_client(int fd, t_inf *inf)
{
  t_client	*previous;
  t_client	*tmp;

  previous = NULL;
  tmp = inf->client;
  while (tmp)
    {
      if (tmp->fd == fd)
	{
	  close(fd);
	  if (previous)
	    previous->next = tmp->next;
	  else
	    inf->client = tmp->next;
	  if (tmp->request.send || tmp->request.get)
	    cancel_requests(tmp);
	  free_client(tmp);
	  return ;
	}
      previous = tmp;
      tmp = tmp->next;
    }
}

void	delete_clients(t_inf *inf)
{
  while (inf->client)
    delete_client(inf->client->fd, inf);
  close(inf->signal);
}
