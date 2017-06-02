/*
** check_ip_port.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Fri Jun  2 09:03:00 2017 Baptiste Veyssiere
** Last update Fri Jun  2 09:11:56 2017 Baptiste Veyssiere
*/

#include "client.h"

int	port_isvalid(const char *port)
{
  int	i;

  i = -1;
  while (port[++i])
    if (port[i] < '0' || port[i] > '9')
      return (-1);
  return (atoi(port));
}

char			ip_isvalid(const char *ip)
{
  struct sockaddr_in	s_in;
  int			res;

  res = inet_pton(AF_INET, ip, &(s_in.sin_addr));
  return (res != 0);
}
