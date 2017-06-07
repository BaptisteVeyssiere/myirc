/*
** pong.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 11:56:53 2017 Baptiste Veyssiere
** Last update Wed Jun  7 16:54:50 2017 Baptiste Veyssiere
*/

#include "client.h"

int	pong(const char *command, t_client *client)
{
  int	i;
  char	*msg;
  int	length;

  printf("Entering pong command\n");
  length = strlen(command);
  if (!(msg = malloc(length + 3)))
    return (1);
  bzero(msg, length + 3);
  i = -1;
  while (command[++i])
    msg[i] = command[i];
  if (command[1] == 'i')
    msg[1] = 'o';
  else if (command[1] == 'I')
    msg[1] = 'O';
  msg[i] = '\r';
  msg[++i] = '\n';
  if (write(client->fd, msg, strlen(msg)) == -1)
    {
      free(msg);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  printf("Return: %s\n", msg);
  free(msg);
  return (0);
}
