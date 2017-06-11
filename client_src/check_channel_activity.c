/*
** check_channel_activity.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sun Jun 11 21:16:01 2017 Baptiste Veyssiere
** Last update Sun Jun 11 21:42:37 2017 Baptiste Veyssiere
*/


#include "client.h"

static char	*get_user(const char *command)
{
  int           i;
  int           length;
  char          *sender;

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

int	check_cjoin(const char *command)
{
  char	*user;

  if (command[0] != ':' || !strstr(command, " JOIN "))
    return (0);
  if (!(user = get_user(command)))
    return (1);
  if (printf("%s join the channel\n", user) < 0)
    {
      free(user);
      return (1);
    }
  free(user);
  return (0);
}

int	check_cpart(const char *command)
{
  char	*user;

  if (command[0] != ':' || !strstr(command, " PART "))
    return (0);
  if (!(user = get_user(command)))
    return (1);
  if (printf("%s leave the channel\n", user) < 0)
    {
      free(user);
      return (1);
    }
  free(user);
  return (0);
}
