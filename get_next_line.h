/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaalouf <jmaalouf@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:55:53 by jmaalouf          #+#    #+#             */
/*   Updated: 2022/05/16 18:09:17 by jmaalouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10
#endif

#include <unistd.h>
#include <stdlib.h>

typedef struct s_circular_buffer
{
	char			buffer[BUFFER_SIZE];
	char			*resizable_storage;
	unsigned int	storage_count;
	unsigned int	malloc_size;
	unsigned int	index_write;
	unsigned int	index_read;
}					t_buffer;

char	*get_next_line (int fd);

#endif