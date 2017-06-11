/*
** file_tools.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 17:15:11 2017 Nathan Scutari
** Last update Sun Jun 11 18:47:23 2017 Nathan Scutari
*/

#include "server.h"

int	check_file_arg_nbr(char *arg, t_client *client, int *tmp)
{
  int	pos;

  if ((pos = get_arg_pos(arg, 3)) == -1 ||
      get_arg_pos(arg, 4) == -1 ||
      get_arg_pos(arg, 5) == -1)
    {
      dprintf(client->fd, ":%s 461 %s :Not enough arguments\r\n",
	      HOSTNAME, client->nick);
      return (-1);
    }
  *tmp = pos;
  return (0);
}

t_client	*check_errors(char *arg, int pos,
			      t_client *client, t_inf *inf)
{
  t_client	*tmp;

  if ((tmp = find_client_by_name(&arg[pos], client, inf)) == NULL)
    {
      dprintf(client->fd, ":%s 141 %s :This user does not exist\r\n",
	      HOSTNAME, client->nick);
      return (NULL);
    }
  else if (client->request.send)
    {
      dprintf(client->fd, ":%s 143 %s :A SEND request is already pending\r\n",
	      HOSTNAME, client->nick);
      return (NULL);
    }
  else if (tmp->request.get)
    {
      dprintf(client->fd,
	      ":%s 144 %s :Target already has a GET request pending\r\n",
	      HOSTNAME, client->nick);
      return (NULL);
    }
  return (tmp);
}

int	send_file(t_client *client, t_inf *inf, char *arg)
{
  int		i;
  int		pos;
  t_client	*tmp;

  if (check_file_arg_nbr(arg, client, &pos) == -1)
    return (1);
  i = pos;
  while (arg[++i] && arg[i] != ' ');
  arg[i] = '\0';
  if ((tmp = check_errors(arg, pos, client, inf)) == NULL)
    return (1);
  arg[i] = ' ';
  tmp->request.get = client;
  client->request.send = tmp;
  dprintf(tmp->fd, ":%s!%s@%s %s\r\n", client->nick,
	  first_arg(client->user), client->hostname,
	  arg);
  return (1);
}
