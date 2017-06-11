/*
** quit.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:46:17 2017 Nathan Scutari
** Last update Sun Jun 11 16:50:26 2017 Nathan Scutari
*/

#include "server.h"

void	inform_quit(t_client *to, t_client *from, char *msg, int pos)
{
  if (pos == -1)
    dprintf(to->fd, ":%s!%s@%s QUIT :Quit: %s\r\n", from->nick,
	    first_arg(from->user), from->hostname, from->nick);
  else
    dprintf(to->fd, ":%s!%s@%s QUIT :Quit: %s\r\n", from->nick,
	    first_arg(from->user), from->hostname, &msg[pos]);
}

int	quit_command(t_client *client, t_inf *inf, char *arg)
{
  t_client	*tmp;
  char		*str;
  int		msg;

  if (client->registered == 0)
    return (err_registered(client, "QUIT"));
  str = &arg[first_arg_pos(arg)];
  msg = get_arg_pos(str, 2);
  tmp = inf->client;
  while (tmp)
    {
      if (tmp->fd != client->fd && in_same_channel(tmp, client))
	inform_quit(tmp, client, str, msg);
      tmp = tmp->next;
    }
  disconnect_client(client, str, msg, inf);
  return (1);
}
