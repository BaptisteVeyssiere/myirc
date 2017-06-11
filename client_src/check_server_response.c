/*
** check_server_response.c for in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Fri Jun  2 11:28:06 2017 Baptiste Veyssiere
** Last update Sun Jun 11 15:08:39 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	read_socket(int fd, t_client *client)
{
  int		ret;
  char		buff[READING_SIZE + 1];
  int		i;

  bzero(buff, READING_SIZE + 1);
  ret = read(fd, buff, READING_SIZE);
  if (ret < 1)
    return (1);
  i = -1;
  while (buff[++i])
    {
      client->buff.data[client->buff.write_ptr] = buff[i];
      if (++(client->buff.write_ptr) == RINGLENGTH)
	client->buff.write_ptr = 0;
    }
  return (0);
}

static int	ring_in_buff(char *buff, char *str, int pos)
{
  int		i;

  i = -1;
  while (1)
    {
      if (str[pos] == '\r' &&
	  str[((pos + 1 == RINGLENGTH) ? 0 : pos + 1)] == '\n')
	{
	  str[pos] = 0;
	  pos = ((pos + 1 == RINGLENGTH) ? 0 : pos + 1);
	  str[pos] = 0;
	  return (1);
	}
      buff[++i] = str[pos];
      str[pos] = 0;
      if (++pos == RINGLENGTH)
	pos = 0;
    }
  buff[++i] = 0;
  buff[++i] = 0;
  return (0);
}

static int	check_ring(t_client *client, char first, char prot)
{
  static char	buff[RINGLENGTH + 1];
  int		tmp;

  bzero(buff, RINGLENGTH);
  tmp = client->buff.read_ptr;
  while ((first == 0 || (client->buff.read_ptr != tmp)) &&
	 client->buff.data[client->buff.read_ptr] != 0)
    {
      first = 1;
      if (client->buff.read_ptr == RINGLENGTH)
	client->buff.read_ptr = 0;
      if (client->buff.data[client->buff.read_ptr] == '\r')
	prot = 1;
      else if (client->buff.data[client->buff.read_ptr] == '\n'
	       && prot == 1)
	{
	  ++client->buff.read_ptr;
	  ring_in_buff(buff, client->buff.data, tmp);
	  return (check_command(buff, client));
	}
      else
	prot = 0;
      ++client->buff.read_ptr;
    }
  client->buff.read_ptr = tmp;
  return (1);
}

static int	check_set(t_client *client, fd_set *set, int server_fd, int signal_fd)
{
  if (FD_ISSET(signal_fd, set))
    return (check_signal(signal_fd));
  else if (FD_ISSET(server_fd, set))
    {
      if (read_socket(server_fd, client) == 1)
	{
	  if (write(1, CONNECTION_LOST, strlen(CONNECTION_LOST)) == -1)
	    return (write_error(__func__, __FILE__, __LINE__));
	  return (2);
	}
      else
	while (check_ring(client, 0, 0) == 0);
    }
  else
    {
      client->user_input = get_next_line(0);
      client->user_on = 1;
    }
  return (0);
}

int			check_server_response(t_client *client, int signal_fd)
{
  fd_set		set;
  static struct timeval	timerange = {0, 0};
  int			ret;

  FD_ZERO(&set);
  if (client->server_on)
    FD_SET(client->fd, &set);
  FD_SET(signal_fd, &set);
  FD_SET(0, &set);
  if ((ret = select(FD_SETSIZE, &set, NULL, NULL, &timerange)) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  if (ret > 0 && (ret = check_set(client, &set, client->fd, signal_fd)))
    return (ret);
  return (0);
}
