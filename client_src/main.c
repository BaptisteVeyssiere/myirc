/*
** main.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 13:41:43 2017 Baptiste Veyssiere
** Last update Thu Jun  1 23:39:26 2017 Baptiste Veyssiere
*/

#include "client.h"

int	main(void)
{
  if (client() != 0)
    return (84);
  return (0);
}
