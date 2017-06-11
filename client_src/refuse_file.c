/*
** refuse_file.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sun Jun 11 01:16:36 2017 Baptiste Veyssiere
** Last update Sun Jun 11 14:57:46 2017 Baptiste Veyssiere
*/

#include "client.h"

int	refuse_file(UNUSED const char **tab, UNUSED const char *src, t_client *client)
{
  if (write(client->fd, "file refuse\r\n", 13) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  if (client->file.name)
    free(client->file.name);
  client->file.name = NULL;
  if (client->file.ip)
    free(client->file.ip);
  client->file.ip = NULL;
  if (client->file.friend)
    free(client->file.friend);
  client->file.friend = NULL;
  return (0);
}
