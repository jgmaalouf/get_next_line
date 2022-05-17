/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaalouf <jmaalouf@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 16:22:44 by jmaalouf          #+#    #+#             */
/*   Updated: 2022/05/17 17:32:59 by jmaalouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_buffer	*buffer_setup()
{
	t_buffer	*ptr;

	ptr = (t_buffer *)malloc(sizeof(t_buffer));
	ptr->index_read = 0;
	ptr->index_write = 0;
	return(ptr);
}

int	storage_setup(t_buffer	*b_ptr)
{
	b_ptr->storage_count = 0;
	b_ptr->malloc_size = BUFFER_SIZE;
	b_ptr->resizable_storage = (char *)malloc(sizeof(char) * b_ptr->malloc_size);
	if (b_ptr->resizable_storage == NULL)
		return (0);
	return (1);
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
	if (bytes_read == 0)
		return (2);
	b_ptr->index_write += bytes_read;
	return (1);
}

int	buffer_read(int fd, t_buffer *b_ptr)
{
	int	check;

	if (b_ptr->index_read == b_ptr->index_write)
	{
		b_ptr->index_read = 0;
		b_ptr->index_write = 0;
	}
	if (b_ptr->index_write == 0)
	{
		check = buffer_write(fd, b_ptr);
		if (check == 0)
			return (0);
		if (check == 2)
			return (2);
	}
	while (b_ptr->index_read < b_ptr->index_write)
	{
		if (storechar(b_ptr) == 0)
			return (0);
		if (b_ptr->buffer[b_ptr->index_read++] == '\n')
			break;
	}
	if (b_ptr->buffer[b_ptr->index_read - 1] != '\n' && b_ptr->index_write == BUFFER_SIZE)
		buffer_read(fd, b_ptr);
	return(1);
}

char	*copy_to_ret(t_buffer *b_ptr)
{
	char			*ret;
	unsigned int	i;

	ret = NULL;
	ret = (char *)malloc(sizeof(char) * (b_ptr->storage_count + 1));
	if (ret != NULL)
	{
		i = 0;
		while (i < b_ptr->storage_count)
		{
			ret[i] = b_ptr->resizable_storage[i];
			i++;
		}
		ret[i] = '\0';
		free(b_ptr->resizable_storage);
		return (ret);
	}
	return (NULL);
}

char	*get_next_line (int fd)
{
	int				check;
	char			*ret;
	static t_buffer	*b_ptr;

	if(b_ptr == NULL)
		b_ptr = buffer_setup();
	if(b_ptr == NULL)
	{
		free(b_ptr);
		return (NULL);
	}
	if(storage_setup(b_ptr) == 0)
		return (NULL);
	check = buffer_read(fd, b_ptr);
	if (check == 0 || check == 2)
	{
		free(b_ptr->resizable_storage);
		free(b_ptr);
		b_ptr = NULL;
		return (NULL);
	}
	ret = copy_to_ret(b_ptr);
	if (ret == NULL)
		return (NULL);
	return (ret);
}

/*
	For using get_next_line with multiple fds:
		- I could make something like a fixed array like: int fd_array[1024];
		- I want to save the values of index_read and index_write to correspond to their specific file descriptor.
		- I can make a function that looks for the file descriptor, and if it doesn't find it we create a new entry for that file descriptor.
	Also clean up your main. You can do it!
*/