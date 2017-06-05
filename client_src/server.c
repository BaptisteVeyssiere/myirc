/*
** server.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 14:47:05 2017 Baptiste Veyssiere
** Last update Tue Jun  6 00:31:48 2017 Baptiste Veyssiere
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

static int		connection_to_server(char *ip, int port,
					     t_client *client)
{
  struct sockaddr_in	s_in;
  struct protoent	*pe;
  int			fd;

  if (port == -1)
    port = 6667;
  if (client->server_on)
    {
      if (write(1, ALREADY_CONNECT, strlen(ALREADY_CONNECT)) < (int)strlen(ALREADY_CONNECT))
	return (write_error(__func__, __FILE__, __LINE__));
      return (0);
    }
  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(port);
  s_in.sin_addr.s_addr = inet_addr(ip);
  if (!(pe = getprotobyname("TCP")) ||
      (fd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  if (connect(fd, (struct sockaddr *)&s_in, sizeof(s_in)) == -1)
    {
      write_error(__func__, __FILE__, __LINE__);
      if (close(fd) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      return (0);
    }
  if (write(1, CONNECTION_ON, strlen(CONNECTION_ON)) < (int)strlen(CONNECTION_ON))
    {
      write_error(__func__, __FILE__, __LINE__);
      if (close(fd) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      return (1);
    }
  client->server_on = 1;
  client->server_name = ip;
  client->fd = fd;
  return (0);
}

int	server(const char **tab, UNUSED const char *src, t_client *client)
{
  char	*ip;
  char	*port;
  int	i;
  int	port_nbr;
  int	ret;

  i = -1;
  port = NULL;
  port_nbr = -1;
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
  if (!ip_isvalid(ip) || (port && (port_nbr = port_isvalid(port)) == -1))
    {
      free(ip);
      free(port);
      return (0);
    }
  ret = connection_to_server(ip, port_nbr, client);
  if (ret)
    free(ip);
  free(port);
  return (ret);
}
