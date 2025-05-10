/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:38:43 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/01 18:55:30 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(char *s)
{
	char	*d_s;
	char	*d;

	d_s = (char *)malloc(ft_strlen(s) + 1);
	if (!d_s)
		return (NULL);
	d = d_s;
	while (*s)
	{
		*d_s = *s;
		d_s++;
		s++;
	}
	*d_s = '\0';
	return (d);
}

char	*ft_strndup(const char *s, int n)
{	
	char	*d_s;
	char	*d;

	d_s = (char *)malloc(ft_strlen(s) + 1);
	if (!d_s)
		return (NULL);
	d = d_s;
	while (*s && n)
	{
		*d_s = *s;
		d_s++;
		s++;
		n--;
	}
	*d_s = '\0';
	return (d);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] - s2[i] != 0)
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	len;

	len = ft_strlen(src);
	if (n == 0)
		return (len);
	if (src)
	{
		while (*src && n > 1)
		{
			*dest = *src;
			dest++;
			src++;
			n--;
		}
	}
	*dest = '\0';
	return (len);
}
