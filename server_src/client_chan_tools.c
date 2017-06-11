/*
** client_chan_tools.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 17:41:00 2017 Nathan Scutari
** Last update Sun Jun 11 17:41:52 2017 Nathan Scutari
*/

#include "server.h"

int	add_chan_to_client(t_channel *chan, t_client *client)
{
  t_join	*tmp;
  t_join	*new_join;

  if ((new_join = malloc(sizeof(t_join))) == NULL)
    return (1);
  new_join->chan = chan;
  new_join->next = NULL;
  if (client->chan == NULL)
    client->chan = new_join;
  else
    {
      tmp = client->chan;
      while (tmp && tmp->next)
	tmp = tmp->next;
      tmp->next = new_join;
    }
  return (0);
}

int	add_client_to_chan(t_client *client, t_channel *chan, int admin)
{
  t_member	*tmp;
  t_member	*parse;

  if ((tmp = malloc(sizeof(t_member))) == NULL)
    return (1);
  tmp->fd = client->fd;
  tmp->admin = admin;
  tmp->next = NULL;
  if (chan->member == NULL)
    chan->member = tmp;
  else
    {
      parse = chan->member;
      while (parse && parse->next)
	{
	  if (parse->fd == client->fd ||
	      parse->next->fd == client->fd)
	    {
	      free(tmp);
	      return (1);
	    }
	  parse = parse->next;
	}
      parse->next = tmp;
    }
  return (add_chan_to_client(chan, client));
}
