/*
** channel_msg.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 01:22:43 2017 Baptiste Veyssiere
** Last update Tue Jun  6 12:07:44 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	get_length(const char *src, const char *channel_name)
{
  int		length;

  length = 0;
  length = strlen(src) + 12 + strlen(channel_name);
  return (length);
}

static void	add_channel_name(char *command, const char *channel_name, int *i)
{
  int		x;

  x = -1;
  while (channel_name[++x])
    command[++(*i)] = channel_name[x];
}

static char	*make_message(const char *src, const char *channel_name)
{
  char		*command;
  int		length;
  int		i;
  int		j;
  char		*base;

  length = get_length(src, channel_name);
  if (!(command = malloc(length)))
    return (NULL);
  bzero(command, length);
  base = "PRIVMSG ";
  i = -1;
  while (base[++i])
    command[i] = base[i];
  --i;
  add_channel_name(command, channel_name, &i);
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

  if (client->channel_name == NULL)
    return (0);
  if (!(command = make_message(src, (const char*)client->channel_name)))
    return (1);
  if (write(client->fd, command, strlen(command)) == -1)
    {
      free(command);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  free(command);
  return (0);
}
