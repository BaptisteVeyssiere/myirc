/*
** check_join.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Wed Jun  7 16:03:55 2017 Baptiste Veyssiere
** Last update Sun Jun 11 16:08:00 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*make_join(const char *username)
{
  char		*command;
  char		*base;
  int	        i;
  int		j;

  base = " JOIN ";
  if (!(command = malloc(strlen(username) + 8)))
    return (NULL);
  bzero(command, strlen(username) + 8);
  i = -1;
  command[0] = ':';
  while (username[++i])
    command[i + 1] = username[i];
  j = -1;
  while (base[++j])
    command[++i] = base[j];
  return (command);
}

static int	free_tmp(char *tmp)
{
  free(tmp);
  return (0);
}

int	check_join(const char *command, t_client *client)
{
  char	*tmp;
  char	*ptr;
  int	i;

  if (client->username == NULL)
    return (0);
  if (!(tmp = make_join(client->username)))
    return (1);
  if (strncmp(command, tmp, strlen(tmp)) != 0)
    return (free_tmp(tmp));
  ptr = (char *)command + strlen(tmp);
  free(tmp);
  tmp = client->channel_name;
  if (!(client->channel_name = malloc(strlen(ptr))))
    {
      client->channel_name = tmp;
      return (1);
    }
  bzero(client->channel_name, strlen(ptr));
  i = -1;
  while (ptr[++i])
    client->channel_name[i] = ptr[i];
  printf("Join channel %s\n", client->channel_name);
  client->waiting_channel = 0;
  return (free_tmp(tmp));
}
