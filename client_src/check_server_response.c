/*
** check_server_response.c for in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Fri Jun  2 11:28:06 2017 Baptiste Veyssiere
** Last update Mon Jun  5 17:04:27 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	check_set(t_client *client, fd_set *set, int server_fd, int signal_fd)
{
  if (FD_ISSET(signal_fd, set))
    return (check_signal(signal_fd));
  else if (FD_ISSET(server_fd, set))
    {
      if (get_response(&client->ringbuffer, server_fd))
	return (1);
      if (extract_response(&client->ringbuffer, &client->response) == 1)
	return (1);
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
  char			msg[MAX_LEN];

  FD_ZERO(&set);
  if (client->server_on)
    FD_SET(client->fd, &set);
  FD_SET(signal_fd, &set);
  FD_SET(0, &set);
  if ((ret = select(FD_SETSIZE, &set, NULL, NULL, &timerange)) == -1)
    {
      snprintf(msg, MAX_LEN, "In function %s, file %s, line %d", __func__, __FILE__,
__LINE__);
      perror(msg);
      return (1);
    }
  if (ret > 0 && check_set(client, &set, client->fd, signal_fd))
    return (1);
  return (0);
}
