/*
** nick_tools.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 15:54:04 2017 Nathan Scutari
** Last update Sun Jun 11 18:48:10 2017 Nathan Scutari
*/

#include "server.h"

int	is_digitletter(char c)
{
  if ((c >= 'a' && c <= 'z') ||
      (c >= 'A' && c <= 'Z') ||
      (c >= '0' && c <= '9'))
    return (1);
  return (0);
}

int	is_special(char c)
{
  int		i;
  static char	specials[] = "-[]\\`^{}";

  i = -1;
  while (specials[++i])
    if (specials[i] == c)
      return (0);
  return (1);
}
