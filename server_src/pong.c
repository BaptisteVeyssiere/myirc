/*
** pong.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:15:22 2017 Nathan Scutari
** Last update Sun Jun 11 16:15:43 2017 Nathan Scutari
*/

#include "server.h"

int	pong_command(t_client *client, t_inf *inf, char *arg)
{
  int	pos;
  char	*str;
  int	i;

  if (client->ping.idle == 0)
    return (0);
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    return (0);
  if (str[pos] == ':')
    ++pos;
  i = pos;
  while (str[++i] && str[i] != ' ');
  str[i] = '\0';
  if (strcmp(&str[pos], inf->hostname) == 0)
    client->ping.idle = 0;
  return (connect_client(client, inf));
}
