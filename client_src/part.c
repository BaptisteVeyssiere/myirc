/*
** part.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 00:25:34 2017 Baptiste Veyssiere
** Last update Tue Jun  6 00:42:20 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*make_part(const char *channel)
{
  char		*ret;
  char		*base;
  int		i;
  int		j;
  int		length;

  base = "part ";
  if (!(ret = malloc(strlen(channel) + 8)))
    return (NULL);
  bzero(ret, strlen(channel) + 8);
  i = -1;
  while (base[++i])
    ret[i] = base[i];
  j = -1;
  --i;
  length = strlen(channel);
  while (++j < length)
    ret[++i] = channel[j];
  ret[++i] = '\r';
  ret[++i] = '\n';
  return (ret);
}

int	part(const char **tab, UNUSED const char *src, t_client *client)
{
  char	*command;

  if (check_param_nbr(tab) < 2)
    {
      if (write(1, INVALID_PARAM, strlen(INVALID_PARAM)) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      return (0);
    }
  else if (tab[1][0] != '#')
    {
      if (write(1, INVALID_CHAN, strlen(INVALID_CHAN)) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      return (0);
    }
  if (!(command = make_part(tab[1])))
    return (1);
  if (write(client->fd, command, strlen(command)) == -1)
    {
      free(command);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  free(command);
  return (0);
}
