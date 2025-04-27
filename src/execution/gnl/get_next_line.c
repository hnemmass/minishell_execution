/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 20:35:56 by hnemmass          #+#    #+#             */
/*   Updated: 2024/11/24 04:16:02 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*result;
	char		*buffer;
	int			i;

	if (fd < 0 || BUFFER_SIZE <= 0 || read (fd, 0, 0) < 0)
		return (free(result), result = NULL, NULL);
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (free(result), free(buffer), result = NULL, NULL);
	i = 1;
	while (i > 0)
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == -1)
			return (NULL);
		buffer[i] = '\0';
		result = alloc(result, buffer);
		if (find(result) == 1)
			break ;
	}
	if (buffer)
		free(buffer);
	buffer = NULL;
	return (crop(&result));
}
