/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaalouf <jmaalouf@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 16:22:44 by jmaalouf          #+#    #+#             */
/*   Updated: 2022/05/21 14:45:44 by jmaalouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	buffer_read_char(t_buffer *buf)
{
	char	c;

	c = buf->buffer[buf->read_index];
	buf->read_index++;
	return (c);
}

int	buffer_reset(t_buffer *buf, int fd)
{
	buf->read_index = 0;
	buf->write_index = read(fd, buf->buffer, BUFFER_SIZE);
	return (buf->write_index > 0);
}

int	buffer_write(int fd, t_buffer *buf)
{
	if (buf->read_index >= buf->write_index)
		if (!buffer_reset(buf, fd))
			return (0);
	return (1);
}

char	*get_next_line(int fd)
{
	static t_buffer	buffer_arr[256];
	t_string		*str;
	char			*ret;

	if (fd < 0 || fd >= 256)
		return (NULL);
	ret = NULL;
	str = string_initialize();
	while (1)
	{
		if (!buffer_write(fd, buffer_arr + fd))
			break ;
		if (!string_store_char(str, buffer_arr + fd))
			return (NULL);
		if (str->string[str->fill_count - 1] == '\n')
			break ;
	}
	string_return(str, &ret);
	string_delete(str);
	return (ret);
}
