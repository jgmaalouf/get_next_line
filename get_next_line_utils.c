/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaalouf <jmaalouf@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 08:29:52 by jmaalouf          #+#    #+#             */
/*   Updated: 2022/05/21 14:47:37 by jmaalouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_string	*string_initialize(void)
{
	t_string	*str;

	str = NULL;
	str = malloc(sizeof(t_string));
	if (str != NULL)
	{
		str->malloc_size = 64;
		str->fill_count = 0;
		str->string = malloc(sizeof(char) * (str->malloc_size));
		if (str->string == NULL)
			str = NULL;
	}
	return (str);
}

void	string_delete(t_string *str)
{
	if (str->string != NULL)
	{
		free(str->string);
		free(str);
	}
}

void	string_return(t_string *str, char **ret)
{
	unsigned int	i;
	char			*tmp;

	tmp = NULL;
	if (str->fill_count != 0)
	{
		tmp = malloc(sizeof(char) * (str->fill_count + 1));
		if (tmp != NULL)
		{
			i = 0;
			while (i < str->fill_count)
			{
				tmp[i] = str->string[i];
				i++;
			}
			tmp[i] = '\0';
		}
	}
	*ret = tmp;
}

int	string_realloc(t_string *str)
{
	char	*new_string;
	size_t	i;

	new_string = NULL;
	new_string = malloc(sizeof(char) * (str->malloc_size * 2));
	if (new_string != NULL)
	{
		i = 0;
		while (i < str->fill_count)
		{
			new_string[i] = str->string[i];
			i++;
		}
		free(str->string);
		str->string = new_string;
		str->malloc_size *= 2;
		return (1);
	}
	return (0);
}

int	string_store_char(t_string *str, t_buffer *buf)
{
	if (str->fill_count >= str->malloc_size)
		if (!string_realloc(str))
			return (0);
	str->string[str->fill_count++] = buffer_read_char(buf);
	return (1);
}
