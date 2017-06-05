/*
** ringbuffer_manager.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 15:02:13 2017 Baptiste Veyssiere
** Last update Mon Jun  5 17:28:17 2017 Baptiste Veyssiere
*/

#include "client.h"

int	get_response(t_ringbuffer *ringbuffer, int server_fd)
{
  char	buffer[READ_SIZE];
  int	i;
  int	read_length;

  bzero(buffer, READ_SIZE);
  if ((read_length = read(server_fd, buffer, READ_SIZE)) < 1)
    return (1);
  i = -1;
  while (++i < read_length)
    {
      if (((i + ringbuffer->write_ptr) % RINGLENGTH) == ringbuffer->read_ptr && ringbuffer->data[ringbuffer->read_ptr])
	ringbuffer->read_ptr = (ringbuffer->read_ptr + 1) % RINGLENGTH;
      ringbuffer->data[(i + ringbuffer->write_ptr) % RINGLENGTH] = buffer[i];
    }
  ringbuffer->write_ptr = (ringbuffer->write_ptr + read_length) % RINGLENGTH;
  write(1, "buffer: ", 8);
  write(1, buffer, strlen(buffer));
  printf("ring: %s\n", ringbuffer->data + ringbuffer->read_ptr);
  return (0);
}
