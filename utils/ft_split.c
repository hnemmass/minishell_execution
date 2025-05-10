/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:03:17 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/20 16:35:28 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_delimiter(char c, const char *delimiters)
{
	int	i;

	i = 0;
	while (delimiters[i])
	{
		if (c == delimiters[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ft_countsep(const char *s, const char *delimiters)
{
	int	count;
	int	w_found;

	count = 0;
	w_found = 0;
	while (*s)
	{
		if (!is_delimiter(*s, delimiters) && !w_found)
		{
			w_found = 1;
			count++;
		}
		else if (is_delimiter(*s, delimiters) && w_found)
			w_found = 0;
		s++;
	}
	return (count);
}

static char	*ft_cpword(const char *s, const char *delimiters)
{
	int		w_sz;
	char	*ret;

	w_sz = 0;
	while (s[w_sz] && !is_delimiter(s[w_sz], delimiters))
		w_sz++;
	ret = (char *)malloc((w_sz + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, s, w_sz + 1);
	return (ret);
}

static void	ft_clear(char **ret)
{
	int	i;

	i = 0;
	while (ret[i])
		free(ret[i++]);
	free(ret);
}

char	**ft_split(char const *s, const char *delimiters)
{
	int		i;
	int		sz;
	char	**ret;

	if (!s)
		return (NULL);
	i = 0;
	sz = ft_countsep(s, delimiters);
	ret = (char **)malloc((sz + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	while (i < sz)
	{
		while (is_delimiter(*s, delimiters))
			s++;
		if (!is_delimiter(*s, delimiters))
			ret[i] = ft_cpword(s, delimiters);
		if (!ret[i])
			return (ft_clear(ret), NULL);
		s += ft_strlen(ret[i++]);
	}
	ret[i] = NULL;
	return (ret);
}
