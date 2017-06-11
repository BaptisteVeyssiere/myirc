/*
** list.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sat Jun 10 17:36:11 2017 Baptiste Veyssiere
** Last update Sun Jun 11 23:21:59 2017 Baptiste Veyssiere
*/

#include "client.h"

int	list(const char **tab, UNUSED const char *src, t_client *client)
{
  int	i;
  int	length;

  if (write(client->fd, "list\r\n", 6) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  if (tab[1])
    {
      length = strlen(tab[1]);
      if (!(client->list_filter = malloc(length + 1)))
	return (1);
      bzero(client->list_filter, length + 1);
      i = -1;
      while (tab[1][++i])
	client->list_filter[i] = tab[1][i];
    }
  return (0);
}

static char	*get_channel(const char *command)
{
  char		*channel;
  int		i;
  int		tmp;
  int		length;

  i = -1;
  length = 0;
  while (command[++i] && command[i] != ' ');
  if (command[i] == 0)
    return (NULL);
  tmp = i;
  while (command[++i] && command[i] != ' ')
    ++length;
  if (!(channel = malloc(length + 2)))
    return (NULL);
  bzero(channel, length + 2);
  channel[length] = '\n';
  i = tmp;
  tmp = -1;
  while (command[++i] && command[i] != ' ')
    channel[++tmp] = command[i];
  return (channel);
}

int	check_list(const char *command, t_client *client)
{
  char	*channel;

  if (strncmp("322 ", command, 4) != 0)
    return (0);
  if (!(channel = get_channel((char *)command + 4)))
    return (1);
  if (client->list_filter == NULL ||
      strstr(channel, client->list_filter))
    {
      if (write(1, channel, strlen(channel)) == -1)
	{
	  free(channel);
	  return (write_error(__func__, __FILE__, __LINE__));
	}
    }
  free(channel);
  return (0);
}

int	check_first_list(const char *command, t_client *client)
{
  if (strncmp("321 ", command, 4) == 0)
    {
      if (printf("Here is the list of the channels on the server:\n") < 0)
	return (write_error(__func__, __FILE__, __LINE__));
      client->waiting_list = 1;
    }
  return (0);
}

int	check_last_list(const char *command, t_client *client)
{
  if (strncmp("323 ", command, 4) == 0)
    {
      client->waiting_list = 0;
      if (client->list_filter)
	{
	  free(client->list_filter);
	  client->list_filter = NULL;
	}
    }
  return (0);
}
