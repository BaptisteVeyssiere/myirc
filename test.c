/*
** test.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Mon Jun  5 17:33:23 2017 Baptiste Veyssiere
** Last update Mon Jun  5 17:49:10 2017 Baptiste Veyssiere
*/

#define RINGLENGTH 256
#define READ_SIZE 50

typedef struct s_ringbuffer
{
  char		data[RINGLENGTH];
  int		read_ptr;
  int		write_ptr;
};

int	reader(t_ringbuffer ringbuffer)
{
  char	buffer[READ_SIZE];

  bzero(buffer, READ_SIZE);

  return (0);
}

int	main()
{
  t_ringbuffer	*ringbuffer;

  bzero(ringbuffer.data, RINGLENGTH);
  ringbuffer.read_ptr = 0;
  ringbuffer.write_ptr = 0;
  reader(ringbuffer);
  return (0);
}
