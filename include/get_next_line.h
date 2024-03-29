/*
** get_next_line.h for get_next_line in /home/VEYSSI_B/rendu/CPE_2015_getnextline
**
** Made by Baptiste veyssiere
** Login   <VEYSSI_B@epitech.net>
**
** Started on  Tue Jan  5 18:07:00 2016 Baptiste veyssiere
** Last update Sun Jun 11 17:48:17 2017 Baptiste Veyssiere
*/

#ifndef GET_NEXT_LINE_H_
# define GET_NEXT_LINE_H_

# include <unistd.h>
# include <stdlib.h>

# ifndef READ_SIZE
#  define READ_SIZE (14)
# endif /* !READ_SIZE */

char	*get_next_line(const int);
char    *my_realloc(char*, int);
int     my_strlen(char*);
char    *read_loop(char*, int, int, char*);
char    *init(char*, char*, int*, int*);
int	end_condition(int *j, int end, char reader, char **line);

#endif /* !GET_NEXT_LINE */
