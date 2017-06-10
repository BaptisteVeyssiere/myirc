/*
** check_part.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  8 01:54:48 2017 Baptiste Veyssiere
** Last update Thu Jun  8 01:58:11 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*make_part(const char *username)
{
  char		*command;
  char		*base;
  int	        i;
  int		j;

  base = " PART ";
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

int	check_part(const char *command, t_client *client)
{
  char	*tmp;
  char	*ptr;
  int	i;

  if (client->username == NULL)
    return (0);
  if (!(tmp = make_part(client->username)))
    return (1);
  if (strncmp(command, tmp, strlen(tmp)) != 0)
    {
      free(tmp);
      return (0);
    }
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
  printf("Leaving channel %s\n", client->channel_name);
  free(tmp);
  client->waiting_channel = 0;
  return (0);
}
