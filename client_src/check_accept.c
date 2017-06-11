/*
** check_refuse.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sun Jun 11 00:54:47 2017 Baptiste Veyssiere
** Last update Sun Jun 11 16:10:51 2017 Baptiste Veyssiere
*/

#include "client.h"

static int		get_client_fd(t_client *client)
{
  int			fd;
  struct sockaddr_in	s_in;
  socklen_t		s_in_size;

  s_in_size = sizeof(s_in);
  if ((fd = accept(client->file.socket_fd,
		   (struct sockaddr*)&s_in, &s_in_size)) == -1)
    {
      if (close(client->file.socket_fd) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      client->file.socket_fd = -1;
      return (write_error(__func__, __FILE__, __LINE__));
    }
  return (fd);
}

static int	send_filedata(t_client *client)
{
  int		fd;
  int		ret;
  char		buff[RINGLENGTH + 1];
  int		client_fd;

  if ((client_fd = get_client_fd(client)) == -1)
    return (1);
  if ((fd = open(client->file.name, O_RDONLY)) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  bzero(buff, RINGLENGTH + 1);
  while ((ret = read(fd, buff, RINGLENGTH)) > 0)
    {
      if (write(client_fd, buff, ret) != ret)
	return (write_error(__func__, __FILE__, __LINE__));
      bzero(buff, RINGLENGTH);
    }
  if (ret == -1 || close(fd) == -1 || close(client_fd) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  if (printf("Transfer successful\n") < 0)
    return (1);
  return (0);
}

int	check_accept(const char *command, t_client *client)
{
  int	i;

  if (client->file.socket_fd < 2 || command[0] != ':')
    return (0);
  i = -1;
  while (command[++i] && command[i] != ' ');
  if (command[i] == 0)
    return (0);
  if (strncasecmp(&command[i], " file get", 9))
    return (0);
  if (printf("The file transfer was accepted\nTransfer in progress...\n") < 0 ||
      send_filedata(client))
    return (1);
  free(client->file.name);
  client->file.name = NULL;
  if (close(client->file.socket_fd) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  client->file.socket_fd = -1;
  return (0);
}
