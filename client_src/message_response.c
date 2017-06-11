/*
** message_response.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Wed Jun  7 22:12:03 2017 Baptiste Veyssiere
** Last update Sun Jun 11 21:27:27 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*make_username(const char *command)
{
  int		i;
  char		*ret;
  int		length;

  i = -1;
  length = 0;
  while (command[++i] && command[i] != '!')
    ++length;
  if (!(ret = malloc(length + 1)))
    return (NULL);
  bzero(ret, length + 1);
  i = -1;
  while (command[++i] && command[i] != '!')
    ret[i] = command[i];
  return (ret);
}

static char	*make_receiver(const char *command, int *i)
{
  char		*ret;
  int		length;
  int		j;

  length = 0;
  j = -1;
  while (command[++j] && command[j] != ' ')
    ++length;
  if (!(ret = malloc(length + 1)))
    return (NULL);
  bzero(ret, length + 1);
  j = -1;
  while (command[++j] && command[j] != ' ')
    ret[++(*i)] = command[j];
  return (ret);
}

int	message_response(const char *command)
{
  char	*username;
  char	*ptr;
  char	*receiver;
  int	i;

  if (!(username = make_username(command + 1)))
    return (1);
  if (!(ptr = strstr(command, " PRIVMSG ")))
    return (1);
  ptr += 9;
  i = -1;
  if (!(receiver = make_receiver(ptr, &i)))
    return (1);
  i += 3;
  if (receiver[0] == '#')
    printf("<%s>: %s\n", username, ptr + i);
  else
    printf("From %s -> %s\n", username, ptr + i);
  free(username);
  free(receiver);
  return (0);
}
