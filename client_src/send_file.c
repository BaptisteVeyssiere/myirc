/*
** send_file.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sat Jun 10 23:26:15 2017 Baptiste Veyssiere
** Last update Sun Jun 11 12:43:47 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	check_file(const char *filename)
{
  struct stat	buf;

  if (stat(filename, &buf) == -1)
    return (1);
  return (!(S_ISREG(buf.st_mode)));
}

static int	initiate_connection(t_client *client)
{
  struct sockaddr_in	s_in;
  struct protoent	*pe;
  int			fd;

  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(4242);
  s_in.sin_addr.s_addr = INADDR_ANY;
  if (!(pe = getprotobyname("TCP")) ||
      (fd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    return (1);
  if (bind(fd, (const struct sockaddr *)&s_in, sizeof(s_in)) == -1 ||
      listen(fd, 3) == -1)
    {
      close(fd);
      return (1);
    }
  client->file.socket_fd = fd;
  return (0);
}

static char	*make_file_command(const char **tab)
{
  char		*command;
  char		*base;
  int		length;
  int		i;
  int		j;

  base = "file send ";
  length = 19 + strlen(tab[2]) + strlen(tab[3]);
  if (!(command = malloc(length)))
    return (NULL);
  bzero(command, length);
  i = -1;
  while (base[++i])
    command[i] = base[i];
  j = -1;
  --i;
  while (tab[2][++j])
    command[++i] = tab[2][j];
  command[++i] = ' ';
  j = -1;
  while (tab[3][++j])
    command[++i] = tab[3][j];
  base = " 4242\r\n";
  j = -1;
  while (base[++j])
    command[++i] = base[j];
  return (command);
}

static char	*get_file_name(const char *path)
{
  int		i;
  char		*ptr;
  char		*name;

  i = -1;
  ptr = (char *)path;
  while (path[++i])
    if (path[i] == '/')
      ptr = (char *)&path[i + 1];
  if (*ptr == 0)
    return (NULL);
  if (!(name = malloc(strlen(ptr) + 1)))
    return (NULL);
  bzero(name, strlen(ptr) + 1);
  i = -1;
  while (ptr[++i])
    name[i] = ptr[i];
  return (name);
}

int	send_file(const char **tab, t_client *client)
{
  char	*command;

  if (check_param_nbr(tab) < 4 || check_file(tab[3]))
    return (0);
  if (initiate_connection(client) == -1 ||
      !(command = make_file_command(tab)))
    return (1);
  if (write(client->fd, command, strlen(command)) == -1)
    {
      free(command);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  free(command);
  if (!(client->file.name = get_file_name(tab[3])))
    return (1);
  return (0);
}
