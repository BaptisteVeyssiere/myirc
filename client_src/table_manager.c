/*
** table_manager.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 00:41:39 2017 Baptiste Veyssiere
** Last update Tue Jun  6 00:45:48 2017 Baptiste Veyssiere
*/

#include "client.h"

int	check_param_nbr(const char **tab)
{
  int	i;

  if (tab == NULL)
    return (0);
  i = -1;
  while (tab[++i]);
  return (i);
}
