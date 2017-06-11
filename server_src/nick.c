/*
** nick.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 15:31:48 2017 Nathan Scutari
** Last update Sun Jun 11 23:37:09 2017 Nathan Scutari
*/

#include "server.h"

int	nick_command(t_client *client, t_inf *inf, char *buff)
{
  char	*nick;
  int	i;
  int	pos;
  char	*old;

  if ((pos = get_arg_pos(buff, 2)) == -1)
    return (send_to_client("431 No nickname given", client));
  i = -1;
  while (buff[pos + ++i] && buff[pos + i] != ' ');
  if ((nick = malloc(i + 1)) == NULL)
    return (1);
  i = -1;
  while (buff[pos + ++i] && buff[pos + i] != ' ')
    nick[i] = buff[pos + i];
  nick[i] = '\0';
  old = client->nick;
  client->nick = nick;
  return (check_nick(client, inf, old));
}

int	nick_err_check(t_client *client, char *old, t_inf *inf)
{
  if (client->nick[0] == '\0')
    {
      send_to_client("431 No nickname given", client);
      client->nick = old;
      return (1);
    }
  if (find_client_by_name(client->nick, client, inf))
    {
      dprintf(client->fd, ":%s 433 %s %s :Nickname is already in use\r\n",
	      HOSTNAME, (old ? old : "*"), client->nick);
      client->nick = old;
      return (1);
    }
  return (0);
}

void	inform_nick(t_client *client, char *old, t_inf *inf)
{
  char		msg[200];
  t_client	*tmp;

  sprintf(msg, ":%s NICK %s\r\n", old, client->nick);
  if (client->registered)
    {
      dprintf(client->fd, "%s", msg);
      tmp = inf->client;
      while (tmp)
	{
	  if (tmp != client && in_same_channel(tmp, client))
	    dprintf(tmp->fd, "%s", msg);
	  tmp = tmp->next;
	}
    }
}

int	check_nick(t_client *client, t_inf *inf, char *old)
{
  int	i;

  i = -1;
  if (nick_err_check(client, old, inf))
    return (0);
  while (client->nick[++i])
    {
      if (!is_digitletter(client->nick[i]) ||
	  !is_special(client->nick[i]))
	{
	  client->nick = old;
	  return (send_to_client("432 Illegal characters", client));
	}
    }
  inform_nick(client, old, inf);
  return (nick_success(client, inf));
}

int	nick_success(t_client *client, t_inf *inf)
{
  if (client->registered == 0 && client->ping.first == 0)
    {
      client->ping.first = 1;
      send_ping(client);
    }
  return (connect_client(client, inf));
}
