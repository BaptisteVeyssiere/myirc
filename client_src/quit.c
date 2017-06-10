/*
** quit.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  8 15:48:43 2017 Baptiste Veyssiere
** Last update Thu Jun  8 16:01:35 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*get_quit(const char *src)
{
  char		*base;
  char		*command;
  int		i;
  int		length;
  int		j;

  base = "quit :";
  i = -1;
  while (src[++i] && src[i] != ' ');
  while (src[++i] && src[i] == ' ');
  length = strlen(&src[i]) + strlen(base) + 3;
  if (!(command = malloc(length)))
    return (NULL);
  bzero(command, length);
  --i;
  j = -1;
  while (base[++j])
    command[j] = base[j];
  --j;
  while (src[++i])
    command[++j] = src[i];
  command[++j] = '\r';
  command[++j] = '\n';
  return (command);
}

int	quit(const char **tab, const char *src, t_client *client)
{
  int	ac;
  char	*msg;

  ac = check_param_nbr(tab);
  if (ac > 1)
    {
      if (!(msg = get_quit(src)))
	return (1);
      if (write(client->fd, msg, strlen(msg)) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      return (3);
    }
  if (write(client->fd, "quit\r\n", 6) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  printf("Disconnected\n");
  return (3);
}
