/*
** users.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Sat Jun 10 17:13:14 2017 Baptiste Veyssiere
** Last update Sat Jun 10 17:50:13 2017 Baptiste Veyssiere
*/

#include "client.h"

int	users(UNUSED const char **tab, UNUSED const char *src, t_client *client)
{
  if (write(client->fd, "users\r\n", 7) == -1)
    return (write_error(__func__, __FILE__, __LINE__));
  return (0);
}

static char	*get_user(const char *command)
{
  char		*user;
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
  if (!(user = malloc(length + 2)))
    return (NULL);
  user[length] = '\n';
  i = tmp;
  tmp = -1;
  while (command[++i] && command[i] != ' ')
    user[++tmp] = command[i];
  return (user);
}

int	check_users(const char *command)
{
  char	*user;

  if (strncmp("393 ", command, 4) != 0)
    return (0);
  if (!(user = get_user((char *)command + 4)))
    return (1);
  if (write(1, user, strlen(user)) == -1)
    {
      free(user);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  free(user);
  return (0);
}

int	check_first_user(const char *command, t_client *client)
{
  if (strncmp("392 ", command, 4) == 0)
    {
      if (printf("Here is the list of the users on the server:\n") < 0)
	return (write_error(__func__, __FILE__, __LINE__));
      client->waiting_users = 1;
    }
  return (0);
}

int	check_last_user(const char *command, t_client *client)
{
  if (strncmp("394 ", command, 4) == 0)
    client->waiting_users = 0;
  return (0);
}
