/*
** arg_tools.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 15:25:16 2017 Nathan Scutari
** Last update Sun Jun 11 15:30:15 2017 Nathan Scutari
*/

#include "server.h"

char	*first_arg(char *user)
{
  int	i;

  i = -1;
  while (user[++i] && user[i] != ' ');
  user[i] = '\0';
  return (user);
}

int	first_arg_pos(char *arg)
{
  int	tmp;
  int	i;

  i = -1;
  while (arg[++i] == ' ');
  if (arg[i] != ':')
    return (i);
  tmp = i;
  --i;
  while (arg[++i] && arg[i] != ' ');
  if (!arg[i])
    return (tmp);
  while (arg[++i] == ' ');
  if (arg[i] == '\0')
    return (tmp);
  return (i);
}

int	get_arg_pos(char *buff, int nbr)
{
  int	i;
  int	arg;

  arg = 0;
  i = first_arg_pos(buff) - 1;
  while (buff[++i])
    {
      if (buff[i] != ' ' && (i == 0 || buff[i - 1] == ' '))
	++arg;
      if (arg == nbr)
	return (i);
    }
  return (-1);
}
