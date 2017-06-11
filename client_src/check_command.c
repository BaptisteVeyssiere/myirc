/*
** check_command.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 22:18:29 2017 Baptiste Veyssiere
** Last update Sun Jun 11 23:17:39 2017 Nathan Scutari
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

static int	checker(const char *epure_command, t_client *client, int ret)
{
  if (check_first_user(epure_command, client) == -1 ||
      check_first_list(epure_command, client) == -1 ||
      check_last_user(epure_command, client) == -1 ||
      check_last_list(epure_command, client) == -1 ||
      (client->waiting_nick && (ret = check_nick(epure_command, client))) ||
      (client->waiting_channel == 1 &&
       (ret = check_join(epure_command, client))) ||
      (client->waiting_channel == -1 &&
       (ret = check_part(epure_command, client))) ||
      (client->waiting_names == 1 &&
       (ret = check_names(epure_command, client))) ||
      (client->waiting_users == 1 && (ret = check_users(epure_command))) ||
      (client->waiting_list == 1 &&
       (ret = check_list(epure_command, client))) ||
      (client->file.name && (ret = check_refuse(epure_command, client))) ||
      (client->file.name && (ret = check_accept(epure_command, client))) ||
      (ret = check_send(epure_command, client)) ||
      (strncasecmp("PING ", epure_command, 5) == 0 &&
       (ret = pong(epure_command, client))) ||
      (strstr(epure_command, "PRIVMSG ") &&
       (ret = message_response(epure_command))) ||
      (ret = check_quit(epure_command)) ||
      (ret = check_cjoin(epure_command)) || (ret = check_cpart(epure_command)))
    return (ret);
  return (0);
}

int	check_command(const char *command, t_client *client)
{
  int	ret;
  char	*epure_command;
  int	shift;

  if (client->first_response)
    return (set_server_name(client, command));
  epure_command = (char *)command;
  shift = 0;
  if (command[0] == ':')
    ++shift;
  if (strncmp(client->server_name, (char *)command + shift,
	      strlen(client->server_name)) == 0)
    epure_command += 1 + shift + strlen(client->server_name);
  if ((ret = checker(epure_command, client, 0)))
    return (ret);
  if (strstr(epure_command, "Nickname is already in use") &&
      printf("The nickname is already used\n") < 0)
    return (1);
  if (strncmp(epure_command, "451 You have not registered", 27) == 0 &&
      printf("Please connect before trying this action\n") < 0)
    return (1);
  return (0);
}
