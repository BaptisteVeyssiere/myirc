/*
** user.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 15:22:16 2017 Nathan Scutari
** Last update Sun Jun 11 23:36:41 2017 Nathan Scutari
*/

#include "server.h"

int	user_command(t_client *client, t_inf *inf, char *arg)
{
  int	i;

  i = -1;
  while (++i < 4)
    if (get_arg_pos(&arg[first_arg_pos(arg)], i + 2) == -1)
      {
	dprintf(client->fd, ":%s 461 %s Not enough parameters\r\n",
		inf->hostname, (client->nick ? client->nick : ""));
	return (0);
      }
  if (client->registered)
    dprintf(client->fd, ":%s 462 %s You may not reregister\r\n",
	    inf->hostname, (client->nick ? client->nick : ""));
  else
    user_in_data(client, &arg[first_arg_pos(arg)]);
  return (connect_client(client, inf));
}

void	user_in_data(t_client *client, char *arg)
{
  int	i;
  int	y;

  if ((client->user = malloc(strlen(&arg[get_arg_pos(arg, 2)]) + 1)) == NULL)
    return ;
  i = get_arg_pos(arg, 2) - 1;
  y = -1;
  while (arg[++i])
    client->user[++y] = arg[i];
  client->user[++y] = '\0';
}
