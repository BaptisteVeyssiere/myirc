/*
** nick.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 22:22:04 2017 Baptiste Veyssiere
** Last update Tue Jun  6 23:01:52 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*make_nick(const char *nickname)
{
  char		*ret;
  char		*base;
  int		i;
  int		j;
  int		length;

  base = "nick ";
  if (!(ret = malloc(strlen(nickname) + 8)))
    return (NULL);
  bzero(ret, strlen(nickname) + 8);
  i = -1;
  while (base[++i])
    ret[i] = base[i];
  j = -1;
  --i;
  length = strlen(nickname);
  while (++j < length)
    ret[++i] = nickname[j];
  ret[++i] = '\r';
  ret[++i] = '\n';
  return (ret);
}

int	nick(const char **tab, UNUSED const char *src, t_client *client)
{
  char	*command;

  if (check_param_nbr(tab) < 2)
    {
      if (write(1, INVALID_PARAM, strlen(INVALID_PARAM)) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      return (0);
    }
  if (!(command = make_nick(tab[1])))
    return (1);
  if (write(client->fd, command, strlen(command)) == -1 ||
      write(client->fd, USER, strlen(USER)) == -1)
    {
      free(command);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  printf("Sending %s\n", command);
  printf("Sending %s\n", USER);
  printf("Waiting for nick %s authorization\n", tab[1]);
  free(command);
  client->waiting_nick = 1;
  return (0);
}
