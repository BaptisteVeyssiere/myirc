/*
** pong.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 11:56:53 2017 Baptiste Veyssiere
** Last update Tue Jun  6 22:28:36 2017 Baptiste Veyssiere
*/

#include "client.h"

int	pong(const char *command)
{
  int	i;
  char	*msg;
  int	length;

  length = strlen(command);
  if (!(msg = malloc(length + 1)))
    return (1);
  bzero(msg, length + 1);
  i = -1;
  while (command[++i])
    msg[i] = command[i];
  if (command[1] == 'i')
    msg[1] = 'o';
  else if (command[1] == 'I')
    msg[1] = 'O';
  return (0);
}
