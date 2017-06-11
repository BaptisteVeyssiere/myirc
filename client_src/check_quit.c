/*
** check_quit.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sun Jun 11 15:21:26 2017 Baptiste Veyssiere
** Last update Sun Jun 11 22:17:41 2017 Baptiste Veyssiere
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

int	check_quit(const char *command)
{
  char	*ptr;
  char	*user;

  if (!(ptr = strstr(command, " QUIT :")))
    return (0);
  if (!(user = get_user(command)))
    return (1);
  ptr += 7;
  if (printf("%s has quit (%s)\n", user, ptr) < 0)
    {
      free(user);
      return (1);
    }
  free(user);
  return (0);
}
