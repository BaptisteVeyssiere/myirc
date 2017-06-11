/*
** ringbuffer.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 14:40:53 2017 Nathan Scutari
** Last update Sun Jun 11 18:52:45 2017 Nathan Scutari
*/

#include "server.h"

int	ring_in_buff(char *buff, t_client *client, int pos, t_inf *inf)
{
  int	i;
  char	*str;

  i = -1;
  ++client->buff.read_ptr;
  str = client->buff.data;
  while (1)
    {
      if (str[pos] == '\r' &&
	  str[((pos + 1 == RINGLENGTH) ? 0 : pos + 1)] == '\n')
	{
	  str[pos] = '\0';
	  pos = ((pos + 1 == RINGLENGTH) ? 0 : pos + 1);
	  str[pos] = '\0';
	  return (check_command(buff, inf, client));
	}
      buff[++i] = str[pos];
      str[pos] = '\0';
      if (++pos == RINGLENGTH)
	pos = 0;
    }
  return (0);
}

int	check_ring(t_client *client, t_inf *inf, char first, char prot)
{
  static char	buff[RINGLENGTH + 1];
  int		tmp;

  bzero(buff, RINGLENGTH);
  tmp = client->buff.read_ptr;
  while ((first == 0 || (client->buff.read_ptr != tmp)) &&
	  client->buff.data[client->buff.read_ptr] != '\0')
    {
      first = 1;
      if (client->buff.read_ptr == RINGLENGTH)
	client->buff.read_ptr = 0;
      if (client->buff.data[client->buff.read_ptr] == '\r')
	prot = 1;
      else if (client->buff.data[client->buff.read_ptr] == '\n'
	       && prot == 1)
	return (ring_in_buff(buff, client, tmp, inf));
      else
	prot = 0;
      ++client->buff.read_ptr;
    }
  client->buff.read_ptr = tmp;
  return (1);
}

int	read_client(int client_fd, fd_set *set, t_inf *inf)
{
  t_client	*client;

  if ((client = get_client(client_fd, inf)) == NULL)
    return (-1);
  if (read_socket(client_fd, client) == -1)
    {
      client_read_error(client, inf);
      delete_client(client_fd, inf);
      FD_CLR(client_fd, set);
      return (0);
    }
  else
    while (check_ring(client, inf, 0, 0) == 0);
  return (0);
}

int	read_socket(int fd, t_client *client)
{
  int		ret;
  char		buff[257];
  int		i;

  bzero(buff, 257);
  ret = read(fd, buff, 256);
  if (ret == -1 || ret == 0)
    return (-1);
  printf("-%s-", buff);
  i = -1;
  while (buff[++i])
    {
      client->buff.data[client->buff.write_ptr] = buff[i];
      if (++client->buff.write_ptr == RINGLENGTH)
	client->buff.write_ptr = 0;
    }
  return (0);
}
