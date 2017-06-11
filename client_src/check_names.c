/*
** check_names.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  8 12:05:08 2017 Baptiste Veyssiere
** Last update Sun Jun 11 15:08:07 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*get_next_name(const char *ptr, int *i)
{
  int		length;
  int		it;
  char		*ret;

  it = *i;
  length = 0;
  while (ptr[++it] && ptr[it] != ' ')
    ++length;
  if (!(ret = malloc(length + 2)))
    return (NULL);
  bzero(ret, length + 2);
  ret[length] = '\n';
  it = -1;
  while (ptr[++(*i)] && ptr[*i] != ' ')
    ret[++it] = ptr[*i];
  return (ret);
}

static int	get_name_nbr(const char *ptr)
{
  int		i;
  int		count;

  i = -1;
  count = 0;
  while (ptr[++i])
    if (i == 0 || (ptr[i] == ' ' && ptr[i + 1] && ptr[i + 1] != ' '))
      ++count;
  return (count);
}

int	check_names(const char *command, t_client *client)
{
  char	*ptr;
  int	i;
  int	count;
  char	*name;

  if (strncmp(command, "353 ", 4))
    return (0);
  if (!(ptr = strstr(command, ":")))
    return (1);
  ++ptr;
  count = get_name_nbr(ptr);
  i = -1;
  if (count < 1)
    return (0);
  if (write(1, NAMES_PROMPT, strlen(NAMES_PROMPT)) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  while (++i < count)
    {
      if (!(name = get_next_name(ptr, &i)))
	return (1);
      if (write(1, name, strlen(name)) == -1)
	{
	  free(name);
	  return (write_error(__func__, __FILE__, __LINE__));
	}
      free(name);
    }
  client->waiting_names = 0;
  return (0);
}
