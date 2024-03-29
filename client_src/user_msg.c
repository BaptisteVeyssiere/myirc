/*
** user_msg.c for Project-Master in /home/veyssi_b/rendu/tek2/PSU/PSU_2016_myirc
**
** Made by Baptiste Veyssiere
** Login   <veyssi_b@epitech.net>
**
** Started on  Tue Jun  6 01:02:40 2017 Baptiste Veyssiere
** Last update Sun Jun 11 17:27:03 2017 Baptiste Veyssiere
*/

#include "client.h"

static char	*get_start(const char *src, int *i)
{
  char		*ptr;
  int		word;

  *i = -1;
  ptr = (char *)src;
  word = 0;
  while (word != 2)
    {
      if (*ptr == ' ' && *(ptr + 1) && *(ptr + 1) != ' ')
	++word;
      ++ptr;
    }
  if (*ptr == ' ')
    ++ptr;
  return (ptr);
}

static char	*make_message(const char **tab, const char *src)
{
  char		*start_ptr;
  char		*command;
  char		*base;
  int		i;
  int		j;

  start_ptr = get_start(src, &i);
  if (!(command = malloc(strlen(start_ptr) + strlen(tab[1]) + 13)))
    return (NULL);
  bzero(command, strlen(start_ptr) + strlen(tab[1]) + 13);
  base = "PRIVMSG ";
  while (base[++i])
    command[i] = base[i];
  j = -1;
  --i;
  while (tab[1][++j])
    command[++i] = tab[1][j];
  command[++i] = ' ';
  command[++i] = ':';
  j = -1;
  while (start_ptr[++j])
    command[++i] = start_ptr[j];
  command[++i] = '\r';
  command[++i] = '\n';
  return (command);
}

int	send_msg_to_user(const char **tab, const char *src, t_client *client)
{
  char	*command;

  if (!(command = make_message(tab, src)))
    return (1);
  if (write(client->fd, command, strlen(command)) == -1)
    {
      free(command);
      return (write_error(__func__, __FILE__, __LINE__));
    }
  free(command);
  return (0);
}
