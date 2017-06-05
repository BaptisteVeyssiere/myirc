/*
** message.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 00:43:46 2017 Baptiste Veyssiere
** Last update Tue Jun  6 01:22:18 2017 Baptiste Veyssiere
*/

#include "client.h"

int	send_file(UNUSED const char **tab, UNUSED t_client *client)
{
  return (0);
}

int	message(const char **tab, const char *src, t_client *client)
{
  int	ac;

  if ((ac = check_param_nbr(tab)) == 0)
    return (0);
  if (strcmp("/msg", tab[0]) == 0 && ac == 4 && tab[1][0] == '#')
    return (send_file(tab, client));
  else if (strcmp("/msg", tab[0]) == 0 && ac < 3)
    {
      if (write(1, INVALID_PARAM, strlen(INVALID_PARAM)) == -1)
	return (write_error(__func__, __FILE__, __LINE__));
      return (0);
    }
  if (strcmp("/msg", tab[0]) == 0)
    return (send_msg_to_user(tab, src, client));
  return (send_msg_to_channel(src, client));
}
