/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaalouf <jmaalouf@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:55:53 by jmaalouf          #+#    #+#             */
/*   Updated: 2022/05/21 14:48:21 by jmaalouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_resizable_string
{
	char			*string;
	unsigned int	malloc_size;
	unsigned int	fill_count;
}					t_string;

typedef struct s_circular_buffer
{
	int		read_index;
	int		write_index;
	char	buffer[BUFFER_SIZE];
}			t_buffer;

char		*get_next_line(int fd);
void		string_delete(t_string *str);
void		string_return(t_string *str, char **ret);
int			string_realloc(t_string *str);
int			string_store_char(t_string *str, t_buffer *buf);
t_string	*string_initialize(void);
char		buffer_read_char(t_buffer *buf);
int			buffer_reset(t_buffer *buf, int fd);
int			buffer_write(int fd, t_buffer *buf);

#endif