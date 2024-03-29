/*
** check_names.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  8 12:05:08 2017 Baptiste Veyssiere
** Last update Sun Jun 11 22:16:34 2017 Baptiste Veyssiere
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
  int	ret;

  if (strncmp(command, "353 ", 4))
    return (0);
  if (!(ptr = strstr(command, ":")) || ++ptr == NULL)
    return (1);
  if ((count = get_name_nbr(ptr)) < 1)
    return (0);
  if (write(1, NAMES_PROMPT, strlen(NAMES_PROMPT)) == -1 || (i = -1) != -1)
    return (write_error(__func__, __FILE__, __LINE__));
  while (--count >= 0)
    {
      if (!(name = get_next_name(ptr, &i)))
	return (1);
      ret = write(1, name, strlen(name));
      free(name);
      if (ret == -1)
	return (write_error(__func__, __FILE__, __LINE__));
    }
  client->waiting_names = 0;
  return (0);
}
