/*
** client.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 14:08:25 2017 Baptiste Veyssiere
** Last update Thu Jun  8 01:54:18 2017 Baptiste Veyssiere
*/

#include "client.h"

static void	free_tab(char **tab)
{
  int		i;

  i = -1;
  while (tab[++i])
    free(tab[i]);
  free(tab);
}

static int	ident_command(const char *line,
			      t_client *client, const char *src)
{
  int		i;
  int		ret;
  char		**tab;
  static char	*command[5] = {
    "/server",
    "/nick",
    "/join",
    "/part",
    "/msg"
  };
  static int	(*fcn_ptr[5])(const char **, const char *, t_client *) =
    {
      server,
      nick,
      join,
      part,
      message
    };

  if (!(tab = strtab(line)))
    return (1);
  if (tab[0] == NULL)
    {
      free(tab);
      return (0);
    }
  i = -1;
  while (++i < 5)
    if (strlen(tab[0]) == strlen(command[i]) &&
	strncmp(tab[0], command[i], strlen(tab[0])) == 0)
      {
	ret = fcn_ptr[i]((const char **)tab, src, client);
	free_tab(tab);
	return (ret);
      }
  if (client->server_on)
    ret = message((const char **)tab, src, client);
  else
    printf("You're not connected\n");
  free_tab(tab);
  return (ret);
}

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
}

int		client(int signal_fd)
{
  char		*line;
  char		*epure;
  t_client	client;

  init_client(&client);
  while (1)
    {
      epure = NULL;
      line = NULL;
      if (check_server_response(&client, signal_fd))
	return (1);
      if (client.user_on)
	{
	  if (!(line = client.user_input))
	    return (0);
	  if (!(epure = epur_str(line)))
	    {
	      free(line);
	      return (1);
	    }
	  if (ident_command(epure, &client, line) == 1)
	    {
	      free(epure);
	      free(line);
	      return (1);
	    }
	  if (line)
	    free(line);
	  if (epure)
	    free(epure);
	  client.user_on = 0;
	}
    }
  return (0);
}
