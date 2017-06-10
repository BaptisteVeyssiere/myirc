/*
** main.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 13:41:43 2017 Baptiste Veyssiere
** Last update Thu Jun  8 15:38:12 2017 Baptiste Veyssiere
*/

#include "client.h"

int	main(void)
{
  int	signal_fd;
  int	ret;

  if ((signal_fd = init_signals()) == -1)
    return (84);
  ret = -1;
  while (ret != 0)
    {
      ret = client(signal_fd);
      if (ret == 2)
	ret = 0;
      if (ret == 1)
	return (84);
    }
  return (0);
}
