/*
** join.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:17:36 2017 Nathan Scutari
** Last update Sun Jun 11 16:23:17 2017 Nathan Scutari
*/

#include "server.h"

int	join_command(t_client *client, t_inf *inf, char *arg)
{
  char	*str;
  int	pos;
  char	*chan;
  int	i;

  if (client->registered == 0)
    return (err_registered(client, "JOIN"));
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    {
      dprintf(client->fd, ":%s 461 %s :Not enough parameters\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  str = &str[pos];
  i = -1;
  while (str[++i] && str[i] != ' ');
  str[i] = '\0';
  while ((chan = strsep(&str, ",")))
    join_chan(inf, client, chan);
  return (0);
}

int	join_chan(t_inf *inf, t_client *client, char *chan_name)
{
  int		admin;
  t_channel	*chan;

  if (chan_name[0] != '#' && chan_name[0] != '&')
    {
      dprintf(client->fd, ":%s 403 %s :No such channel\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  admin = 0;
  if ((chan = find_chan(chan_name, inf)) == NULL &&
      (admin = 1 == 1) &&
      (chan = create_chan(chan_name, inf)) == NULL)
    return (-1);
  if (add_client_to_chan(client, chan, admin))
    return (1);
  inform_all_join(chan, client);
  inform_client_join(chan, client, inf);
  return (0);
}

void	inform_all_join(t_channel *chan, t_client *client)
{
  t_member	*tmp;

  tmp = chan->member;
  while (tmp)
    {
      dprintf(tmp->fd, ":%s!%s@%s JOIN %s\r\n",
	      client->nick, first_arg(client->user),
	      client->hostname, chan->name);
      tmp = tmp->next;
    }
}
