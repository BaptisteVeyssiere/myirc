/*
** accept_file.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sat Jun 10 23:58:15 2017 Baptiste Veyssiere
** Last update Sun Jun 11 15:06:59 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	read_data(const int fd, t_client *client)
{
  int		file_fd;
  char		buff[RINGLENGTH + 1];
  int		ret;

  bzero(buff, RINGLENGTH + 1);
  if ((file_fd = open(client->file.name, O_CREAT | O_TRUNC | O_WRONLY, 0755)) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  while ((ret = read(fd, buff, RINGLENGTH)) > 0)
    {
      if (write(file_fd, buff, ret) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      bzero(buff, RINGLENGTH);
    }
  if (ret == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  if (close(file_fd) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  return (0);
}

static int		waiting_data(t_client *client)
{
  struct sockaddr_in	s_in;
  struct protoent	*pe;
  int			fd;
  char			*ip;

  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(4242);
  ip = strcmp("localhost", client->file.ip) == 0 ? "127.0.0.1" : client->file.ip;
  s_in.sin_addr.s_addr = inet_addr(ip);
  if (!(pe = getprotobyname("TCP")) ||
      (fd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  if (connect(fd, (struct sockaddr *)&s_in, sizeof(s_in)) == -1)
    {
      if (close(fd) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      return (write_error(__func__, __FILE__, __LINE__));
    }
  if (printf("Reception in progress...\n") < 0 ||
      read_data(fd, client) ||
      printf("Reception successful\n") < 0)
    return (1);
  if (close(fd) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  return (0);
}

int	accept_file(const char **tab, UNUSED const char *src, t_client *client)
{
  if (check_param_nbr(tab) < 2)
    {
      if (printf("Not enough arguments\n") < 0)
	return (1);
      return (0);
    }
  if (strcmp(client->file.friend, tab[1]))
    {
      if (printf("%s didn't send you a file\n", tab[1]) < 0)
	return (1);
      return (0);
    }
  if (write(client->fd, "file get\r\n", 10) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  if (waiting_data(client))
    return (1);
  free(client->file.ip);
  free(client->file.friend);
  free(client->file.name);
  client->file.friend = NULL;
  client->file.ip = NULL;
  client->file.name = NULL;
  return (0);
}
