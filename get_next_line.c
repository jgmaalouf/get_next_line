/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaalouf <jmaalouf@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 08:51:34 by jmaalouf          #+#    #+#             */
/*   Updated: 2022/05/16 21:12:26 by jmaalouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
/*
	- SICP: Structure and Interpretation of Computer Programs (Harold Abelson and Gerald J. Sussman)
	- Elements of Artificial Intelligence: A Modern Approach (Peter Norvig)
	- Goedel, Escher, Bach (Douglas R. Hofstadter) 
	
*/

#include <stdio.h>
#include <fcntl.h>
t_buffer	*buffer_setup()
{
	t_buffer	*ptr;

	ptr = (t_buffer *)malloc(sizeof(t_buffer));
	ptr->index_read = 0;
	ptr->index_write = 0;
	ptr->malloc_size = BUFFER_SIZE;
	ptr->resizable_storage = (char *)malloc(sizeof(char) * ptr->malloc_size);
	if (ptr->resizable_storage == NULL)
		return(NULL);
	return(ptr);
}

int	realloc_storage(t_buffer *b_ptr)
{
	char			*new_storage;
	unsigned int	i;

	b_ptr->malloc_size *= 2;
	new_storage = NULL;
	new_storage = (char *)malloc(sizeof(char) * b_ptr->malloc_size);
	if (new_storage != NULL)
	{
		i = 0;
		while (i < b_ptr->storage_count)
		{
			new_storage[i] = b_ptr->resizable_storage[i];
			i++;
		}
		free(b_ptr->resizable_storage);
		b_ptr->resizable_storage = new_storage;
		return (1);
	}
	return (0);
}

int	storechar(t_buffer *b_ptr)
{
	if (b_ptr->storage_count == b_ptr->malloc_size)
	{
		if (realloc_storage(b_ptr) == 0)
			return (0);
	}
	b_ptr->resizable_storage[b_ptr->storage_count] = b_ptr->buffer[b_ptr->index_read];
	b_ptr->storage_count++;
	return (1);
}

int	buffer_write(int fd, t_buffer *b_ptr)
{
	int	bytes_read;
	
	bytes_read = read(fd, b_ptr->buffer, BUFFER_SIZE);
	if (bytes_read == -1)
		return (0);
	b_ptr->index_write += bytes_read;
	return (1);
}

int	buffer_read(int fd, t_buffer *b_ptr)
{
	if (buffer_write(fd, b_ptr) == 0)
		return (0);
	while (b_ptr->index_read < b_ptr->index_write)
	{
		if (storechar(b_ptr) == 0)
			return (0);
		if (b_ptr->buffer[b_ptr->index_read++] == '\n')
			break;
	}
	if (b_ptr->index_read - 1 != '\n' && b_ptr->index_write == BUFFER_SIZE)
	{
		b_ptr->index_read = 0;
		b_ptr->index_write = 0;
		buffer_read(fd, b_ptr);
	}
	return(1);
}

char	*copy_to_ret(t_buffer *b_ptr)
{
	char			*ret;
	unsigned int	i;

	ret = NULL;
	ret = (char *)malloc(sizeof(char) * b_ptr->storage_count + 1);
	if (ret != NULL)
	{
		i = 0;
		while (i < b_ptr->storage_count)
		{
			ret[i] = b_ptr->resizable_storage[i];
			i++;
		}
		ret[i] = '\0';
		b_ptr->storage_count = 0;
		return (ret);
	}
	return (NULL);
}

char	*get_next_line (int fd)
{
	char			*ret;
	static t_buffer	*b_ptr;

	if(b_ptr == NULL)
		b_ptr = buffer_setup();
	if(b_ptr == NULL)
	{
		free(b_ptr);
		return (NULL);
	}
	if (buffer_read(fd, b_ptr) == 0)
		return (NULL);
	ret = copy_to_ret(b_ptr);
	if (ret == NULL)
		return (NULL);
	return (ret);
}

int main (void)
{
	int fd = open("text.txt", O_RDONLY);
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
}