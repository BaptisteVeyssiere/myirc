/*
** names.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:41:36 2017 Nathan Scutari
** Last update Sun Jun 11 17:42:34 2017 Nathan Scutari
*/

#include "server.h"

void	inform_client_join(t_channel *chan, t_client *client, t_inf *inf)
{
  t_client	*tmp;
  t_member	*parse;

  dprintf(client->fd, ":%s 353 %s = %s ", HOSTNAME,
	  client->nick, chan->name);
  parse = chan->member;
  if ((tmp = get_client(parse->fd, inf)) != NULL)
    dprintf(client->fd, ":%s%s", (parse->admin == 1 ? "@" : ""),
	    tmp->nick);
  parse = parse->next;
  while (parse)
    {
      if ((tmp = get_client(parse->fd, inf)) != NULL)
	dprintf(client->fd, " %s%s", (parse->admin == 1 ? "@" : ""),
		tmp->nick);
      parse = parse->next;
    }
  dprintf(client->fd, "\r\n:%s 366 %s %s :End of /NAMES list.\r\n",
	 HOSTNAME, client->nick, chan->name);
}

void	name_chan(char *chan, t_client *client, t_inf *inf)
{
  t_channel	*channel;

  if ((channel = find_chan(chan, inf)))
    inform_client_join(channel, client, inf);
  else
    dprintf(client->fd, ":%s 401 %s %s :No such channel\r\n",
	    HOSTNAME, client->nick, chan);
}

int	names_command(t_client *client, t_inf *inf, char *arg)
{
  int		pos;
  char		*str;
  int		i;
  char		*chan;

  if (client->registered == 0)
    return (err_registered(client, "NAMES"));
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    {
      dprintf(client->fd, ":%s 366 %s * :End of /NAMES list\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  str = &str[pos];
  i = -1;
  while (str[++i] && str[i] != ' ');
  str[i] = '\0';
  while ((chan = strsep(&str, ",")))
    name_chan(chan, client, inf);
  return (0);
}
