/*
** check_waiting.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 11:39:22 2017 Baptiste Veyssiere
** Last update Tue Jun  6 22:34:24 2017 Baptiste Veyssiere
*/

#include "client.h"


int	check_nick(const char *command, t_client *client)
{
  int	i;
  int	length;
  char	*ptr;
  char	*tmp;

  if (!(ptr = strstr(command, " 001 ")))
    return (0);
  ptr += 5;
  tmp = ptr;
  i = -1;
  length = 0;
  while (tmp[++i] && tmp[i] != '\n' && tmp[i] != '\r' && tmp[i] != ' ')
    ++length;
  i = -1;
  tmp = client->nickname;
  if (!(client->nickname = malloc(length + 1)))
    return (1);
  bzero(client->nickname, length + 1);
  while (ptr[++i])
    client->nickname[i] = ptr[i];
  if (tmp)
    free(tmp);
  printf("You're now known as %s\n", client->nickname);
  return (0);
}

int	check_add_channel(const char *command, UNUSED t_client *client)
{
  printf("%s\n", command);
  return (0);
}

int	check_del_channel(const char *command, UNUSED t_client *client)
{
  printf("%s\n", command);
  return (0);
}
