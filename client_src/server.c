/*
** server.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 14:47:05 2017 Baptiste Veyssiere
** Last update Thu Jun  1 23:50:31 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*get_host(const char *s)
{
  int		length;
  int		i;
  char		*host;

  length = 0;
  i = -1;
  while (s[++i] && s[i] != ':')
    ++length;
  if (!(host = malloc(length + 1)))
    return (NULL);
  bzero(host, length + 1);
  i = -1;
  while (s[++i] && s[i] != ':')
    host[i] = s[i];
  return (host);
}

static char	*get_port(const char *s)
{
  char		*port;
  int		i;
  int		tmp;
  int		length;

  i = -1;
  length = 0;
  while (s[++i] && s[i] != ':');
  tmp = i;
  while (s[++i])
    ++length;
  if (!(port = malloc(length + 1)))
    return (NULL);
  bzero(port, length + 1);
  i = -1;
  while (s[++tmp])
    port[++i] = s[tmp];
  return (port);
}

int	server(const char **tab, const char *src, t_client *client)
{
  char	*ip;
  char	*port;
  int	i;

  (void)client;
  (void)src;
  i = -1;
  port = NULL;
  while (tab[++i]);
  if (i != 2)
    {
      write(1, "Invalid number of parameters\n", 29);
      return (0);
    }
  if (!(ip = get_host(tab[1])))
    return (1);
  if (strlen(ip) < strlen(tab[1]) &&
      !(port = get_port(tab[1])))
    {
      free(ip);
      return (1);
    }
  write(1, ip, strlen(ip));
  write(1, "\n", 1);
  if (port)
    {
      write(1, port, strlen(port));
      write(1, "\n", 1);
    }
  free(ip);
  free(port);
  return (0);
}
