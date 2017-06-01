/*
** ringbuffer.h for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 13:48:15 2017 Baptiste Veyssiere
** Last update Thu Jun  1 13:49:56 2017 Baptiste Veyssiere
*/

#ifndef RINGBUFFER_H_
# define RINGBUFFER_H_

# define RINGLENGTH	4096

typedef struct		s_ringbuffer
{
  char			data[RINGLENGTH];
  int			write_ptr;
  int			read_ptr;
}			t_ringbuffer;

#endif /* !RINGBUFFER_H_ */
