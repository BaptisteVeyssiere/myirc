/*
** init_server.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 14:24:56 2017 Nathan Scutari
** Last update Sun Jun 11 14:33:04 2017 Nathan Scutari
*/

#include "server.h"

int	get_server_socket(char *arg)
{
  struct sockaddr_in	s_in;
  struct protoent	*pe;
  int			i;
  int			port;
  int			fd;

  i = -1;
  while (arg[++i])
    if (arg[i] < '0' || arg[i] > '9')
      return (-1);
  port = atoi(arg);
  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(port);
  s_in.sin_addr.s_addr = INADDR_ANY;
  if (!(pe = getprotobyname("TCP")))
    return (print_err("getprotobybyname failed\n", -1));
  if ((fd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    return (print_err("socket failed\n", -1));
  if (bind(fd, (const struct sockaddr *)&s_in, sizeof(s_in)) == -1)
    {
      close(fd);
      return (print_err("bind failed\n", -1));
    }
  return (fd);
}

int	init_signals()
{
  int		sfd;
  sigset_t	mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0 ||
      (sfd = signalfd(-1, &mask, 0)) < 0)
    return (-1);
  return (sfd);
}
