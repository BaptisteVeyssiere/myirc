/*
** check_waiting.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 11:39:22 2017 Baptiste Veyssiere
** Last update Sun Jun 11 21:50:36 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*get_username(const char *command)
{
  char		*username;
  int		i;
  char		*ptr;

  if (!(ptr = strstr(command, ":Welcome ")))
    return (NULL);
  ptr += 9;
  i = -1;
  if (!(username = malloc(strlen(ptr) + 1)))
    return (NULL);
  bzero(username, strlen(ptr) + 1);
  while (ptr[++i])
    username[i] = ptr[i];
  return (username);
}

static int	get_length(char *tmp)
{
  int		length;
  int		i;

  i = -1;
  length = 0;
  while (tmp[++i] && tmp[i] != '\n' && tmp[i] != '\r' && tmp[i] != ' ')
    ++length;
  return (length);
}

int	check_nick(const char *command, t_client *client)
{
  int	i;
  int	length;
  char	*ptr;
  char	*tmp;

  if (strncmp(command, "001 ", 4))
    return (check_rename(command, client));
  ptr = (char *)command + 4;
  length = get_length(ptr);
  i = -1;
  tmp = client->nickname;
  if (!(client->nickname = malloc(length + 1)))
    return (1);
  bzero(client->nickname, length + 1);
  while (ptr[++i] && ptr[i] != '\n' && ptr[i] != '\r' && ptr[i] != ' ')
    client->nickname[i] = ptr[i];
  if (tmp)
    free(tmp);
  printf("You're now known as %s\n", client->nickname);
  if (client->username)
    free(client->username);
  if (!(client->username = get_username(command)))
    return (1);
  client->waiting_nick = 0;
  return (0);
}
