/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 06:02:07 by hnemmass          #+#    #+#             */
/*   Updated: 2024/11/24 04:30:08 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

int	find(char *result)
{
	int	i;

	if (!result)
		return (0);
	i = 0;
	while (result[i])
	{
		if (result[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*alloc(char *result, char *buffer)
{
	char	*new;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(buffer);
	if (!result)
		return (crop_till(buffer, len));
	len += ft_strlen(result);
	new = malloc(len + 1);
	if (!new)
		return (free(result), NULL);
	while (result[i])
	{
		new[i] = result[i];
		i++;
	}
	while (buffer[i - ft_strlen(result)])
	{
		new[i] = buffer[i - ft_strlen(result)];
		i++;
	}
	new[i] = '\0';
	free(result);
	return (new);
}

char	*crop_till(char *tmp, int i)
{
	char	*new;
	int		j;

	if (!tmp || !i)
		return (NULL);
	j = 0;
	new = malloc(i + 1);
	if (!new)
		return (NULL);
	while (j < i)
	{
		new[j] = tmp[j];
		j++;
	}
	new[j] = '\0';
	return (new);
}

char	*crop(char **result)
{
	char	*tmp;
	char	*line;
	int		i;

	if (!*result)
		return (NULL);
	tmp = *result;
	i = 0;
	while (tmp[i] && tmp[i] != '\n')
		i++;
	if (tmp[i] == '\n')
		i++;
	line = crop_till(tmp, i);
	if (!line)
		return (free(*result), *result = NULL, NULL);
	*result = crop_till(tmp + i, ft_strlen(tmp + i));
	free(tmp);
	tmp = NULL;
	return (line);
}
