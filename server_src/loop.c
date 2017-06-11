/*
** loop.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 14:36:47 2017 Nathan Scutari
** Last update Sun Jun 11 18:47:46 2017 Nathan Scutari
*/

#include "server.h"

int	server_loop(t_inf *inf)
{
  fd_set		set;
  fd_set		try;
  struct timeval	timerange;
  int			ret;

  FD_ZERO(&set);
  FD_SET(inf->server, &set);
  FD_SET(inf->signal, &set);
  inf->set = &set;
  timerange.tv_sec = 0;
  timerange.tv_usec = 0;
  while (1)
    {
      try = set;
      if ((ret = select(FD_SETSIZE, &try, NULL, NULL, &timerange)) == -1)
	return (1);
      if (ret > 0)
	if (check_set(&try, inf, &set))
	  return (0);
      update_timers(inf, &set);
      usleep(100);
    }
  return (0);
}

int	check_set(fd_set *try, t_inf *inf, fd_set *set)
{
  int	i;

  i = -1;
  if (FD_ISSET(inf->signal, try))
    return (check_signal(inf->signal));
  else if (FD_ISSET(inf->server, try))
    accept_new_client(set, inf);
  else
    {
      while (++i < FD_SETSIZE)
	{
	  if (FD_ISSET(i, try))
	    read_client(i, set, inf);
	}
    }
  return (0);
}

int	check_signal(int sfd)
{
  struct signalfd_siginfo	si;

  read(sfd, &si, sizeof(si));
  if (si.ssi_signo == SIGINT || si.ssi_signo == SIGTERM)
    {
      printf("Signal caught, closing.\n");
      return (1);
    }
  return (0);
}
