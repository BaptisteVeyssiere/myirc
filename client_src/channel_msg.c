/*
** channel_msg.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 01:22:43 2017 Baptiste Veyssiere
** Last update Tue Jun  6 01:44:57 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	get_length(const char *src, const char **channel_list)
{
  int		length;
  int		i;

  length = 0;
  i = -1;
  if (channel_list)
    while (channel_list[++i])
      length += strlen(channel_list[i]) + 1;
  length += strlen(src) + 12;
  return (length);
}

static void	add_channel_list(char *command, const char **channel_list, int *i)
{
  int		x;
  int		y;

  y = -1;
  if (channel_list)
    while (channel_list[++y])
      {
	x = -1;
	if (y > 0)
	  command[++(*i)] = ',';
	while (channel_list[y][++x])
	  command[++(*i)] = channel_list[y][x];
      }
}

static char	*make_message(const char *src, const char **channel_list)
{
  char		*command;
  int		length;
  int		i;
  int		j;
  char		*base;

  length = get_length(src, channel_list);
  if (!(command = malloc(length)))
    return (NULL);
  bzero(command, length);
  base = "PRIVMSG ";
  i = -1;
  while (base[++i])
    command[i] = base[i];
  --i;
  add_channel_list(command, channel_list, &i);
  j = -1;
  command[++i] = ' ';
  command[++i] = ':';
  while (src[++j])
    command[++i] = src[j];
  command[++i] = '\r';
  command[++i] = '\n';
  return (command);
}

int	send_msg_to_channel(const char *src, t_client *client)
{
  char	*command;

  if (check_param_nbr((const char **)client->channel_list) == 0)
    return (0);
  if (!(command = make_message(src, (const char**)client->channel_list)))
    return (1);
  if (write(client->fd, command, strlen(command)) == -1)
    {
      free(command);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  free(command);
  return (0);
}
