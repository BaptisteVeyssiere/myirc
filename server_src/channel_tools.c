/*
** channel_tools.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 15:03:13 2017 Nathan Scutari
** Last update Sun Jun 11 15:08:36 2017 Nathan Scutari
*/

#include "server.h"

int	count_users(t_channel *chan)
{
  int		nbr;
  t_member	*tmp;

  nbr = 0;
  tmp = chan->member;
  while (tmp)
    {
      ++nbr;
      tmp = tmp->next;
    }
  return (nbr);
}

t_channel	*find_chan(char *str, t_inf *inf)
{
  t_channel	*tmp;

  tmp = inf->channel;
  while (tmp)
    {
      if (strcasecmp(tmp->name, str) == 0)
	return (tmp);
      tmp = tmp->next;
    }
  return (NULL);
}

void	send_custom_to_chan(t_client *client, t_channel *chan, char *txt)
{
  t_member	*tmp;

  tmp = chan->member;
  while (tmp)
    {
      if (tmp->fd != client->fd)
	dprintf(tmp->fd, "%s\r\n", txt);
      tmp = tmp->next;
    }
}

void	remove_chan(t_channel *chan, t_inf *inf)
{
  t_channel	*prev;
  t_channel	*tmp;

  tmp = inf->channel;
  prev = NULL;
  while (tmp)
    {
      if (tmp == chan)
	{
	  if (prev == NULL)
	    inf->channel = tmp->next;
	  else
	    prev->next = tmp->next;
	  free(tmp);
	  return ;
	}
      prev = tmp;
      tmp = tmp->next;
    }
}

t_channel	*create_chan(char *str, t_inf *inf)
{
  t_channel	*tmp;
  t_channel	*new_chan;

  if ((new_chan = malloc(sizeof(t_channel))) == NULL)
    return (NULL);
  if ((new_chan->name = strdup(str)) == NULL)
    return (NULL);
  new_chan->member = NULL;
  new_chan->next = NULL;
  if (inf->channel == NULL)
    inf->channel = new_chan;
  else
    {
      tmp = inf->channel;
      while (tmp && tmp->next)
	tmp = tmp->next;
      tmp->next = new_chan;
    }
  return (new_chan);
}
