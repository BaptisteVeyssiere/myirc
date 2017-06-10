/*
** write_error.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 22:26:34 2017 Baptiste Veyssiere
** Last update Tue Jun  6 00:18:30 2017 Baptiste Veyssiere
*/

#include "client.h"

int	write_error(const char *func, const char *file, int line)
{
  char	msg[MAX_LEN];

  snprintf(msg, MAX_LEN, "In function %s, file %s, line %d", func, file, line);
  perror(msg);
  return (1);
}
