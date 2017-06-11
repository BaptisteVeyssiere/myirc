/*
** privmsg.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:26:25 2017 Nathan Scutari
** Last update Sun Jun 11 21:24:39 2017 Nathan Scutari
*/

#include "server.h"

int	privmsg_arg_check(int *pos, int *txt, char **str, t_client *client)
{
  if ((*pos = get_arg_pos(*str, 2)) == -1)
    {
      dprintf(client->fd, ":%s 411 %s :No recipient given\r\n",
	      HOSTNAME, client->nick);
      return (1);
    }
  *str = &(*str)[*pos];
  if ((*txt = get_arg_pos(*str, 2)) == -1)
    {
      dprintf(client->fd, "%s 412 %s :No text to send\r\n",
	      HOSTNAME, client->nick);
      return (1);
    }
  return (0);
}

int	privmsg_command(t_client *client, t_inf *inf, char *arg)
{
  char	*str;
  char	*msg;
  char	*to;
  int	pos;
  int	txt;
  int	i;

  if (client->registered == 0)
    return (err_registered(client, "PRIVMSG"));
  str = &arg[first_arg_pos(arg)];
  if (privmsg_arg_check(&pos, &txt, &str, client))
    return (0);
  i = -1;
  while (str[++i] && str[i] != ' ');
  str[i] = '\0';
  if ((msg = strdup(&str[txt])) == NULL)
    return (-1);
  while ((to = strsep(&str, ",")))
    sendprivmsg(client, to, msg, inf);
  free(msg);
  return (0);
}

int	sendprivmsg(t_client *client, char *to, char *str, t_inf *inf)
{
  t_channel	*chan;
  t_client	*tmp;

  if ((chan = find_chan(to, inf)))
    send_to_chan(client, chan, str);
  else if ((tmp = find_client_by_name(to, NULL, inf)))
    send_private(client, tmp, str);
  else
    dprintf(client->fd, ":%s 401 %s :No such nick/channel\r\n",
	    HOSTNAME, client->nick);
  return (0);
}

int	send_to_chan(t_client *client, t_channel *chan,
		     char *txt)
{
  t_member	*tmp;

  tmp = chan->member;
  while (tmp)
    {
      if (tmp->fd != client->fd)
	{
	  dprintf(tmp->fd, ":%s!%s@%s PRIVMSG %s :%s\r\n",
		  client->nick, first_arg(client->user),
		  client->hostname,
		  chan->name, (txt[0] == ':' ? &txt[1] : txt));
	}
      tmp = tmp->next;
    }
  return (0);
}

int	send_private(t_client *from, t_client *to, char *txt)
{
  dprintf(to->fd, ":%s!%s@%s PRIVMSG %s :%s\r\n",
	  from->nick, first_arg(from->user), from->hostname,
	  to->nick, (txt[0] == ':' ? &txt[1] : txt));
  return (0);
}
