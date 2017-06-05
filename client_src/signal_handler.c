/*
** signal_handler.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Fri Jun  2 10:59:53 2017 Baptiste Veyssiere
** Last update Mon Jun  5 17:05:40 2017 Baptiste Veyssiere
*/

#include "client.h"

int				check_signal(int fd)
{
  struct signalfd_siginfo	si;
  char				msg[MAX_LEN];

  if (read(fd, &si, sizeof(si)) < (int)sizeof(si))
    {
      snprintf(msg, MAX_LEN, "In function %s, file %s, line %d", __func__, __FILE__, __LINE__);
      perror(msg);
      return (1);
    }
  if (si.ssi_signo == SIGINT || si.ssi_signo == SIGTERM)
    {
      if (write(2, SIGNAL_CAUGHT, strlen(SIGNAL_CAUGHT)) < (int)strlen(SIGNAL_CAUGHT))
	{
	  snprintf(msg, MAX_LEN, "In function %s, file %s, line %d", __func__, __FILE__, __LINE__);
	  perror(msg);
	}
      return (1);
    }
  return (0);
}

int		init_signals(void)
{
  int		fd;
  sigset_t	mask;
  char		msg[MAX_LEN];

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0 ||
      (fd = signalfd(-1, &mask, 0)) < 0)
    {
      snprintf(msg, MAX_LEN, "In function %s, file %s, line %d", __func__, __FILE__, __LINE__);
      perror(msg);
      return (-1);
    }
  return (fd);
}
