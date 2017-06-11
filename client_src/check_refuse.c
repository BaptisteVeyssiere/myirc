/*
** check_refuse.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sun Jun 11 00:54:47 2017 Baptiste Veyssiere
** Last update Sun Jun 11 15:00:15 2017 Baptiste Veyssiere
*/

#include "client.h"

int	check_refuse(const char *command, t_client *client)
{
  int	i;

  if (client->file.socket_fd < 2 || command[0] != ':')
    return (0);
  i = -1;
  while (command[++i] && command[i] != ' ');
  if (command[i] == 0)
    return (0);
  if (strncasecmp(&command[i], " file refuse", 12))
    return (0);
  if (printf("The file transfer was refused\n") < 0)
    return (1);
  free(client->file.name);
  client->file.name = NULL;
  if (close(client->file.socket_fd) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  client->file.socket_fd = -1;
  return (0);
}
