/*
** file.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:59:25 2017 Nathan Scutari
** Last update Sun Jun 11 18:17:53 2017 Nathan Scutari
*/

#include "server.h"

int	get_file(t_client *client)
{
  if (client->request.get == NULL)
    {
      dprintf(client->fd, ":%s 142 %s :No pending file request\r\n",
	      HOSTNAME, client->nick);
      return (1);
    }
  dprintf(client->request.get->fd, ":%s!%s@%s FILE GET\r\n",
	  client->nick, first_arg(client->user), client->hostname);
  client->request.get->request.send = NULL;
  client->request.get = NULL;
  return (1);
}

int	refuse_file(t_client *client)
{
  if (client->request.get == NULL)
    {
      dprintf(client->fd, ":%s 142 %s :No pending file request\r\n",
	      HOSTNAME, client->nick);
      return (1);
    }
  dprintf(client->request.get->fd, ":%s!%s@%s FILE REFUSE\r\n",
	  client->nick, first_arg(client->user), client->hostname);
  client->request.get->request.send = NULL;
  client->request.get = NULL;
  return (1);
}

int	file_check_arg(char *arg, int pos, t_client *client, t_inf *inf)
{
  int	i;
  char	save;

  i = pos;
  while (arg[++i] && arg[i] != ' ');
  save = arg[i];
  arg[i] = '\0';
  if (strcasecmp(&arg[pos], "SEND") == 0)
    {
      arg[i] = save;
      return (send_file(client, inf, arg));
    }
  else if (strcasecmp(&arg[pos], "GET") == 0)
    {
      arg[i] = save;
      return (get_file(client));
    }
  else if (strcasecmp(&arg[pos], "REFUSE") == 0)
    return (refuse_file(client));
  return (0);
}

int	file_command(t_client *client, t_inf *inf, char *arg)
{
  int	pos;

  if (client->registered == 0)
    return (err_registered(client, "FILE"));
  if ((pos = get_arg_pos(arg, 2)) == -1)
    {
      dprintf(client->fd, ":%s 461 %s :Not enough arguments\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  if (file_check_arg(arg, pos, client, inf))
    return (0);
  dprintf(client->fd, ":%s 140 %s :Wrong argument\r\n",
	  HOSTNAME, client->nick);
  return (0);
}
