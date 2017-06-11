/*
** client.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 14:08:25 2017 Baptiste Veyssiere
** Last update Sun Jun 11 17:03:04 2017 Baptiste Veyssiere
*/

#include "client.h"

static void	init_client(t_client *client)
{
  client->server_on = 0;
  client->server_name = NULL;
  client->nickname = NULL;
  client->fd = -1;
  client->buff.write_ptr = 0;
  client->buff.read_ptr = 0;
  bzero(client->buff.data, RINGLENGTH);
  client->user_input = NULL;
  client->user_on = 0;
  client->channel_name = NULL;
  client->waiting_nick = 0;
  client->waiting_channel = 0;
  client->first_response = 0;
  client->username = NULL;
  client->waiting_users = 0;
  client->waiting_list = 0;
  client->list_filter = NULL;
  client->file.socket_fd = -1;
  client->file.name = NULL;
  client->file.ip = NULL;
  client->file.friend = NULL;
}

static int	free_client(t_client *client, int ret)
{
  if (client->server_name)
    free(client->server_name);
  if (client->nickname)
    free(client->nickname);
  return (ret);
}

static int	checker(char **line, char **epure, t_client *client, int *r)
{
  int		ret;

  if (!(*line = client->user_input))
    {
      *r = 1;
      return (free_client(client, 0));
    }
  if (!(*epure = epur_str(*line)))
    {
      free(*line);
      *r = 1;
      return (free_client(client, 1));
    }
  if ((ret = ident_command(*epure, client, *line)) == 1 || ret == 3)
    {
      *r = 1;
      free(*epure);
      free(*line);
      return (free_client(client, ret));
    }
  if (*line)
    free(*line);
  if (*epure)
    free(*epure);
  return (0);
}

int		client(int signal_fd)
{
  char		*line;
  char		*epure;
  t_client	client;
  int		ret;
  int		r;

  init_client(&client);
  while (1)
    {
      epure = NULL;
      line = NULL;
      if ((ret = check_server_response(&client, signal_fd)))
	return (free_client(&client, ret));
      if (client.user_on)
	{
	  ret = 0;
	  r = checker(&line, &epure, &client, &ret);
	  if (ret == 1)
	    return (r);
	  client.user_on = 0;
	}
    }
  return (free_client(&client, 0));
}
