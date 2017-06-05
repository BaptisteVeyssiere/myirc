/*
** check_command.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 22:18:29 2017 Baptiste Veyssiere
** Last update Tue Jun  6 00:19:02 2017 Baptiste Veyssiere
*/

#include "client.h"

int	check_command(const char *command, UNUSED t_client *client)
{
  printf("%s\n", command);
  return (0);
}
