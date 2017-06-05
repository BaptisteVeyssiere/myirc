/*
** signal_handler.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Fri Jun  2 10:59:53 2017 Baptiste Veyssiere
** Last update Tue Jun  6 00:32:46 2017 Baptiste Veyssiere
*/

#include "client.h"

int				check_signal(int fd)
{
  struct signalfd_siginfo	si;

  if (read(fd, &si, sizeof(si)) < (int)sizeof(si))
    return (write_error(__func__, __FILE__, __LINE__));
  if (si.ssi_signo == SIGINT || si.ssi_signo == SIGTERM)
    {
      if (write(2, SIGNAL_CAUGHT, strlen(SIGNAL_CAUGHT)) < (int)strlen(SIGNAL_CAUGHT))
	return (write_error(__func__, __FILE__, __LINE__));
      return (1);
    }
  return (0);
}

int		init_signals(void)
{
  int		fd;
  sigset_t	mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0 ||
      (fd = signalfd(-1, &mask, 0)) < 0)
    return (write_error(__func__, __FILE__, __LINE__));
  return (fd);
}
