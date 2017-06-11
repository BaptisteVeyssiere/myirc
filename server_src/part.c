/*
** part.c for myirc in /home/scutar_n/rendu/PSU/PSU_2016_myirc/server_src
**
** Made by Nathan Scutari
** Login   <scutar_n@epitech.net>
**
** Started on  Sun Jun 11 16:37:30 2017 Nathan Scutari
** Last update Sun Jun 11 16:41:20 2017 Nathan Scutari
*/

#include "server.h"

int	part_command(t_client *client, t_inf *inf, char *arg)
{
  char	*str;
  char	*chan;
  int	pos;
  int	msg;
  int	i;

  if (client->registered == 0)
    return (err_registered(client, "PART"));
  str = &arg[first_arg_pos(arg)];
  if ((pos = get_arg_pos(str, 2)) == -1)
    {
      dprintf(client->fd, ":%s 461 %s PART :Not enough parameters\r\n",
	      HOSTNAME, client->nick);
      return (0);
    }
  msg = get_arg_pos(str, 3);
  str = &str[pos];
  i = -1;
  while (str[++i] && str[i] != '\0');
  str[i] = '\0';
  while ((chan = strsep(&str, ",")))
    leave_chan(client, chan, msg, inf);
  return (0);
}

void	leave_chan(t_client *client, char *chan, int msg, t_inf *inf)
{
  t_channel	*tmp;
  char		buff[200];

  if ((tmp = find_chan(chan, inf)) == NULL)
    {
      dprintf(client->fd, ":%s 403 %s :No such channel\r\n",
	       HOSTNAME, client->nick);
      return ;
    }
  else if (msg == -1)
    sprintf(buff, ":%s!%s@%s PART %s\r\n", client->nick,
	    first_arg(client->user), client->hostname,
	    tmp->name);
  else
    sprintf(buff, ":%s!%s@%s PART %s :%s", client->nick,
	    first_arg(client->user), client->hostname,
	    tmp->name, (chan[msg] == ':' ? &chan[msg + 1] : &chan[msg]));
  send_custom_to_chan(client, tmp, buff);
  dprintf(client->fd, "%s", buff);
  delete_client_from_chan(client, tmp, inf);
}
