/*
** ident_command.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc/client_src
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sun Jun 11 16:19:10 2017 Baptiste Veyssiere
** Last update Sun Jun 11 17:42:02 2017 Baptiste Veyssiere
*/

#include "client.h"

static int	free_tab(char **tab)
{
  int           i;

  i = -1;
  if (tab)
    {
      while (tab[++i])
	free(tab[i]);
      free(tab);
    }
  return (0);
}

static void	check_command_name(char **tab, int *ret,
			      const char *src, t_client *client)
{
  int		i;
  static char   *command[11] =
    {
      "/server", "/nick", "/join", "/part", "/msg", "/names",
      "/quit", "/users", "/list", "/refuse", "/accept_file"
    };
  static int    (*fcn_ptr[11])(const char **, const char *, t_client *) =
    {
      server, nick, join, part, message, names, quit,
      users, list, refuse_file, accept_file
    };

  i = -1;
  while (++i < 11)
    {
      if (strlen(tab[0]) == strlen(command[i]) &&
	  strncmp(tab[0], command[i], strlen(tab[0])) == 0)
	{
	  *ret = fcn_ptr[i]((const char **)tab, src, client);
	  free_tab(tab);
	  return ;
      }
    }
}

int		ident_command(const char *line,
                              t_client *client, const char *src)
{
  int           ret;
  char          **tab;

  if (!(tab = strtab(line)))
    return (1);
  if (tab[0] == NULL)
    return (free_tab(tab));
  ret = 0;
  check_command_name(tab, &ret, src, client);
  if (ret)
    return (ret);
  if (client->server_on)
    ret = message((const char **)tab, src, client);
  else if (printf("Please connect before trying this action\n") < 0)
    return (1);
  free_tab(tab);
  return (ret);
}
