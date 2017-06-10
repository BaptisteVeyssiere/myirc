/*
** check_command.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 22:18:29 2017 Baptiste Veyssiere
** Last update Sat Jun 10 17:50:59 2017 Baptiste Veyssiere
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
  int	shift;

  printf("Handling command: <%s>\n", command);
  if (client->first_response)
    return (set_server_name(client, command));
  epure_command = (char *)command;
  shift = 0;
  if (command[0] == ':')
    ++shift;
  if (strncmp(client->server_name, (char *)command + shift,
	      strlen(client->server_name)) == 0)
    epure_command += 1 + shift + strlen(client->server_name);
  if (check_first_user(epure_command, client) == -1 ||
      check_first_list(epure_command, client) == -1 ||
      check_last_user(epure_command, client) == -1 ||
      check_last_list(epure_command, client) == -1 ||
      (client->waiting_nick && (ret = check_nick(epure_command, client))) ||
      (client->waiting_channel == 1 && (ret = check_join(epure_command, client))) ||
      (client->waiting_channel == -1 && (ret = check_part(epure_command, client))) ||
      (client->waiting_names == 1 && (ret = check_names(epure_command, client))) ||
      (client->waiting_users == 1 && (ret = check_users(epure_command))) ||
      (client->waiting_list == 1 && (ret = check_list(epure_command, client))) ||
      (strncasecmp("PING ", epure_command, 5) == 0 && (ret = pong(epure_command, client))) ||
      (strstr(epure_command, "PRIVMSG ") && (ret = message_response(epure_command))))
    return (ret);
  return (0);
}
