/*
** client.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Thu Jun  1 14:08:25 2017 Baptiste Veyssiere
** Last update Fri Jun  2 10:33:56 2017 Baptiste Veyssiere
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
  static char	*command[1] = { "/server" };
  static int	(*fcn_ptr[1])(const char **, const char *, t_client *) =
    {
      server
    };

  if (!(tab = strtab(line)))
    return (1);
  if (tab[0] == NULL)
    {
      free(tab);
      return (0);
    }
  i = -1;
  while (++i < 1)
    if (strlen(tab[0]) == strlen(command[i]) &&
	strncmp(tab[0], command[i], strlen(tab[0])) == 0)
      {
	ret = fcn_ptr[i]((const char **)tab, src, client);
	free_tab(tab);
	return (ret);
      }
  free_tab(tab);
  (void)client;
  return (0);
}

static void	init_client(t_client *client)
{
  client->server_on = 0;
  client->server_name = NULL;
  client->nickname = NULL;
  client->fd = -1;
}

int		client(void)
{
  char		*line;
  char		*epure;
  t_client	client;

  init_client(&client);
  while (1)
    {
      epure = NULL;
      line = NULL;
      if (!(line = get_next_line(0)))
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
    }
  return (0);
}
