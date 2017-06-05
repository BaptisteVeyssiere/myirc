/*
** main.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 13:41:43 2017 Baptiste Veyssiere
** Last update Fri Jun  2 11:25:10 2017 Baptiste Veyssiere
*/

#include "client.h"

int	main(void)
{
  int	signal_fd;

  if ((signal_fd = init_signals()) == -1)
    return (84);
  if (client(signal_fd) != 0)
    return (84);
  return (0);
}
