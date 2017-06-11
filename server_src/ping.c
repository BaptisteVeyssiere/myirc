/*
** ping.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:12:15 2017 Nathan Scutari
** Last update Sun Jun 11 16:15:06 2017 Nathan Scutari
*/

#include "server.h"

int	err_registered(t_client *client, char *cmd)
{
  dprintf(client->fd, ":%s 451 %s :You have not registered\r\n",
	  HOSTNAME, cmd);
  return (0);
}

int	ping_command(t_client *client, t_inf *inf, char *arg)
{
  int	ret;
  int	pos;
  int	i;

  if (client->registered == 0)
    return (err_registered(client, "PING"));
  pos = first_arg_pos(arg);
  if ((ret = get_arg_pos(&arg[pos], 2)) == -1)
    {
      dprintf(client->fd, ":%s 409 %s missing argument\r\n",
	      inf->hostname, (client->nick ? client->nick : ""));
      return (0);
    }
  pos += ret;
  i = pos;
  while (arg[++i] && arg[i] != ' ');
  arg[i] = '\0';
  dprintf(client->fd, ":%s PONG %s :%s\r\n",
	  inf->hostname, inf->hostname,
	  (arg[pos] == ':' ? &arg[pos + 1] : &arg[pos]));
  return (0);
}
