/*
** check_command.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 22:18:29 2017 Baptiste Veyssiere
** Last update Tue Jun  6 23:06:06 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	set_server_name(t_client *client, const char *command)
{
  int		i;
  int		length;

  if (command[0] != ':')
    return (0);
  length = 0;
  i = 0;
  while (command[++i] != ' ')
    ++length;
  if (!(client->server_name = malloc(length + 1)))
    return (1);
  bzero(client->server_name, length + 1);
  i = 0;
  while (command[++i] != ' ')
    client->server_name[i - 1] = command[i];
  client->first_response = 0;
  return (0);
}

int	check_command(const char *command, t_client *client)
{
  int	ret;
  char	*epure_command;

  write(1, command, strlen(command));
  if (client->first_response)
    return (set_server_name(client, command));
  if (command[0] == ':')
    epure_command = (char *)command + 2 + strlen(client->server_name);
  else
    epure_command = (char *)command;
  if ((client->waiting_nick && (ret = check_nick(epure_command, client))) ||
      (client->waiting_channel == 1 && (ret = check_add_channel(epure_command, client))) ||
      (client->waiting_channel == -1 && (ret = check_del_channel(epure_command, client))) ||
      (strncasecmp("PING ", epure_command, 5) == 0 && (ret = pong(epure_command))))
    return (ret);
  return (0);
}
