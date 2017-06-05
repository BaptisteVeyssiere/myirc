/*
** extract_response.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 15:54:34 2017 Baptiste Veyssiere
** Last update Mon Jun  5 16:28:48 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*get_server_response(t_ringbuffer *ringbuffer, int length)
{
  int		i;
  char		*ret;

  i = -1;
  if (!(ret = malloc(length)))
    return (NULL);
  bzero(ret, length);
  while ((++i < (length - 1)))
    ret[i] = ringbuffer->data[(i + ringbuffer->read_ptr) % RINGLENGTH];
  while (ringbuffer->data[ringbuffer->read_ptr] != '\r' &&
	 ringbuffer->data[(ringbuffer->read_ptr + 1) % RINGLENGTH] != '\n')
    {
      ringbuffer->data[ringbuffer->read_ptr] = 0;
      ringbuffer->read_ptr = (ringbuffer->read_ptr + 1) % RINGLENGTH;
    }
  ringbuffer->data[ringbuffer->read_ptr] = 0;
  ringbuffer->data[(ringbuffer->read_ptr + 1) % RINGLENGTH] = 0;
  ringbuffer->read_ptr = (ringbuffer->read_ptr + 2) % RINGLENGTH;
  return (ret);
}

int	extract_response(t_ringbuffer *ringbuffer, char **response)
{
  int	i;
  int	length;
  int	stop;
  char	*ret;

  ret = NULL;
  i = ringbuffer->read_ptr - 1;
  length = 0;
  stop = 0;
  while (!stop && (++i % RINGLENGTH) != ringbuffer->write_ptr)
    {
      if (ringbuffer->data[i % RINGLENGTH] == '\n' &&
	  ringbuffer->data[i % RINGLENGTH - 1] == '\r')
	stop = 1;
      else
	++length;
    }
  if (stop && !(ret = get_server_response(ringbuffer, length)))
    return (1);
  *response = ret;
  write(1, *response, strlen(*response));
  write(1, "\n", 1);
  return (0);
}
