/*
** epur_str.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 23:04:19 2017 Baptiste Veyssiere
** Last update Thu Jun  1 23:04:20 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	get_full_length(const char *s)
{
  int		i;
  int		length;

  i = -1;
  length = 0;
  while (s[++i] && s[i] == ' ');
  if (s[i])
    ++length;
  while (s[++i])
    if (s[i] != ' ' || (s[i] == ' ' && s[i + 1] && s[i + 1] != ' '))
      ++length;
  return (length);
}

char	*epur_str(const char *s)
{
  int	length;
  char	*ret;
  int	i;
  int	j;

  if (!s)
    return (NULL);
  length = get_full_length(s);
  if (!(ret = malloc(length + 1)))
    return (NULL);
  bzero(ret, length + 1);
  i = -1;
  j = -1;
  while (s[++i] && s[i] == ' ');
  --i;
  while (s[++i])
    if (s[i] != ' ' || (s[i] == ' ' && s[i + 1] && s[i + 1] != ' '))
      ret[++j] = s[i];
  return (ret);
}
