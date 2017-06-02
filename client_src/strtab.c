/*
** strtab.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 23:22:31 2017 Baptiste Veyssiere
** Last update Thu Jun  1 23:22:40 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	count_words(const char *s)
{
  int		ret;
  int		i;

  ret = 0;
  i = -1;
  while (s[++i])
    if (i == 0 || (s[i] != ' ' && s[i - 1] == ' '))
      ++ret;
  return (ret);
}

static int	get_word_length(const char *s)
{
  int		i;
  int		length;

  i = -1;
  length = 0;
  while (s[++i] && s[i] != ' ')
    ++length;
  return (length);
}

static char	*get_word(const char *s, int *it)
{
  char		*ret;
  int		length;
  int		i;

  length = get_word_length(s + *it + 1);
  if (!(ret = malloc(length + 1)))
    return (NULL);
  bzero(ret, length + 1);
  i = -1;
  while (s[++(*it)] && s[*it] != ' ')
    ret[++i] = s[*it];
  return (ret);
}

char	**strtab(const char *s)
{
  int	nbr;
  char	**ret;
  int	it;
  int	i;

  nbr = count_words(s);
  if (!(ret = malloc(sizeof(char*) * (nbr + 1))))
    return (NULL);
  ret[nbr] = NULL;
  it = -1;
  i = -1;
  while (++i < nbr)
    if (!(ret[i] = get_word(s, &it)))
      {
	while (--i >= 0)
	  free(ret[i]);
	free(ret);
	return (NULL);
      }
  return (ret);
}
