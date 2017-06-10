/*
** names.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  8 11:53:01 2017 Baptiste Veyssiere
** Last update Thu Jun  8 15:55:20 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*make_names_command(const char *command)
{
  int		i;
  int		j;
  char		*base;
  char		*ret;
  int		length;

  base = "names ";
  i = -1;
  length = strlen(command) + strlen(base);
  if (!(ret = malloc(length + 3)))
    return (NULL);
  bzero(ret, length + 3);
  while (base[++i])
    ret[i] = base[i];
  j = -1;
  --i;
  while (command[++j])
    ret[++i] = command[j];
  ret[++i] = '\r';
  ret[++i] = '\n';
  return (ret);
}

int	names(const char **tab, UNUSED const char *src, t_client *client)
{
  int	ac;
  char	*command;

  if ((ac = check_param_nbr(tab)) < 2)
    return (0);
  if (!(command = make_names_command(tab[1])))
    return (1);
  if (write(client->fd, command, strlen(command)) == -1)
    {
      free(command);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  free(command);
  client->waiting_names = 1;
  return (0);
}
