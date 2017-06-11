/*
** write_error.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 22:26:34 2017 Baptiste Veyssiere
** Last update Sun Jun 11 17:48:47 2017 Baptiste Veyssiere
*/

#include "client.h"

int	write_error(const char *func, const char *file, int line)
{
  char	msg[MAX_LEN];

  snprintf(msg, MAX_LEN, "In function %s, file %s, line %d", func, file, line);
  perror(msg);
  return (1);
}

int	free_server(char *ip, char *port)
{
  free(ip);
  free(port);
  return (0);
}

int	end_condition(int *j, int end, char reader, char **line)
{
  *j = -1;
  if (end == 0 && reader == 0)
    if ((*line = my_realloc(*line, (my_strlen(*line) + READ_SIZE))) == NULL)
      return (-1);
  return (0);
}
