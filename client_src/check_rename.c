/*
** check_rename.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sun Jun 11 21:42:14 2017 Baptiste Veyssiere
** Last update Sun Jun 11 22:17:52 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*get_old(const char *command)
{
  int		i;
  int		length;
  char		*name;

  i = 0;
  length = 0;
  while (command[++i] && command[i] != ' ')
    ++length;
  if (!command[i] || !(name = malloc(length + 1)))
    return (NULL);
  bzero(name, length + 1);
  i = 0;
  while (command[++i] && command[i] != ' ')
    name[i - 1] = command[i];
  return (name);
}

static char	*get_new(const char *command)
{
  int		i;
  int		length;
  char		*name;

  i = -1;
  length = 0;
  while (command[++i])
    ++length;
  if (!(name = malloc(length + 1)))
    return (NULL);
  bzero(name, length + 1);
  i = -1;
  while (command[++i])
    name[i] = command[i];
  return (name);
}

static int	change_username(t_client *client)
{
  int		i;
  int		length;
  int		tmp;
  char		*ptr;

  i = -1;
  length = 0;
  while (client->username[++i] && client->username[i] != '!')
    ++length;
  tmp = i - 1;
  length = strlen(client->username) + strlen(client->nickname) - length;
  if (!(ptr = malloc(length + 1)))
    return (1);
  bzero(ptr, length + 1);
  i = -1;
  while (client->nickname[++i])
    ptr[i] = client->nickname[i];
  --i;
  while (client->username[++tmp])
    ptr[++i] = client->username[tmp];
  free(client->username);
  client->username = ptr;
  return (0);
}

int	check_rename(const char *command, t_client *client)
{
  char	*ptr;
  char	*new;
  char	*old;

  if (command[0] != ':' ||
      strncmp(&command[1], client->nickname, strlen(client->nickname)) ||
      strncmp(&command[strlen(client->nickname) + 1], " NICK ", 6) ||
      !(ptr = strstr(command, "NICK ")))
    return (0);
  ptr += 5;
  if (!(old = get_old(command)) || !(new = get_new(ptr)) ||
      printf("%s is now known as %s\n", old, new) < 0)
    return (1);
  if (client->nickname)
    free(client->nickname);
  free(old);
  client->nickname = new;
  if (change_username(client))
    return (1);
  client->waiting_nick = 0;
  return (0);
}
