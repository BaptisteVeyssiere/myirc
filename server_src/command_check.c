/*
** command_check.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 14:57:24 2017 Nathan Scutari
** Last update Sun Jun 11 14:59:08 2017 Nathan Scutari
*/

#include "server.h"

int	check_command(char *buff, t_inf *inf, t_client *client)
{
  int		i;
  static char	*commands[] =
    {
      "NICK", "USER", "PING", "PONG", "JOIN", "PRIVMSG",
      "PART", "NAMES", "QUIT", "LIST", "USERS", "FILE", 0
    };
  static int	(*fnc[])(t_client *, t_inf *, char *) =
    {
      nick_command, user_command, ping_command, pong_command,
      join_command, privmsg_command, part_command, names_command,
      quit_command, list_command, users_command, file_command
    };

  printf("%s -- ", buff);
  printf("%d\n", client->fd);
  if (buff[0] == '\0')
    return (0);
  printf("k\n");
  i = -1;
  while (commands[++i])
    if (command_cmp(commands[i], buff, first_arg_pos(buff)))
      return (fnc[i](client, inf, buff));
  return (bad_command(inf, client));
}

int	command_cmp(char *command, char *str, int pos)
{
  int	i;

  i = -1;
  while (command[++i])
    if (command[i] != toupper(str[pos + i]))
      return (0);
  if (str[pos + i] != ' ' && str[pos + i] != '\0')
    return (0);
  return (1);
}

int	bad_command(t_inf *inf, t_client *client)
{
  if (client->registered == 0)
    dprintf(client->fd, ":%s 451 You have not registered\r\n",
	    inf->hostname);
  else
    dprintf(client->fd, ":%s 421 Unknown command\r\n",
	    inf->hostname);
  return (0);
}
