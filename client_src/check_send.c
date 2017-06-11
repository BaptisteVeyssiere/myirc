/*
** check_send.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sun Jun 11 00:09:32 2017 Baptiste Veyssiere
** Last update Sun Jun 11 17:13:12 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	check_if_good_command(const char *command)
{
  int		i;

  i = -1;
  if (command[0] != ':')
    return (0);
  while (command[++i] && command[i] != ' ');
  if (!command[i])
    return (0);
  while (command[++i] && command[i] == ' ');
  if (!command[i])
    return (0);
  if (strncmp(&command[i], "file send ", 10))
    return (0);
  return (1);
}

static char	*get_sender(const char *command)
{
  int		i;
  int		length;
  char		*sender;

  i = 0;
  length = 0;
  while (command[++i] && command[i] != '!')
    ++length;
  if (!(sender = malloc(length + 1)))
    return (NULL);
  bzero(sender, length + 1);
  i = 0;
  while (command[++i] && command[i] != '!')
    sender[i - 1] = command[i];
  return (sender);
}

static char	*get_file(const char *command)
{
  int		i;
  char		*ptr;
  char		*file;
  int		length;
  int		tmp;

  i = -1;
  if (!(ptr = strstr(command, "send ")))
    return (NULL);
  length = 0;
  ptr += 5;
  while (ptr[++i] && ptr[i] != ' ');
  if (ptr[i] == 0)
    return (NULL);
  tmp = i;
  while (ptr[++i] && ptr[i] != ' ')
    ++length;
  if (!(file = malloc(length + 1)))
    return (NULL);
  bzero(file, length + 1);
  i = tmp;
  tmp = -1;
  while (ptr[++i] && ptr[i] != ' ')
    file[++tmp] = ptr[i];
  return (file);
}

static char	*get_ip(const char *command)
{
  char		*ip;
  int		i;
  int		length;
  char		*ptr;

  length = 0;
  if (!(ptr = strstr(command, "@")))
    return (NULL);
  ++ptr;
  i = -1;
  while (ptr[++i] && ptr[i] != ' ')
    ++length;
  if (!(ip = malloc(length + 1)))
    return (NULL);
  bzero(ip, length + 1);
  i = -1;
  while (ptr[++i] && ptr[i] != ' ')
    ip[i] = ptr[i];
  return (ip);
}

int	check_send(const char *command, t_client *client)
{
  if (check_if_good_command(command) == 0)
    return (0);
  if (client->file.friend)
    free(client->file.friend);
  if (client->file.name)
    free(client->file.name);
  if (client->file.ip)
    free(client->file.ip);
  if (!(client->file.friend = get_sender(command)) ||
      !(client->file.name = get_file(command)) ||
      !(client->file.ip = get_ip(command)))
    return (1);
  if (printf("%s want to send you the file %s\n",
	     client->file.friend, client->file.name) < 0)
    {
      free(client->file.friend);
      free(client->file.name);
      free(client->file.ip);
      return (1);
    }
  return (0);
}
