/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:47:34 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/25 20:37:19 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strconcat(const char *s1, const char *s2)
{
	int		i;
	int		j;
	int		s1_sz;
	int		s2_sz;
	char	*ret;

	if (!s1 && !s2)
		return (ft_strdup(""));
	ret = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			ret[i] = s1[i];
			i++;
		}
	}
	j = 0;
	while (s2[j])
	{
		ret[i] = s2[j];
		i++;
		j++;
	}
	ret[i] = '\0';
	return (ret);
}

static int	is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r'
        || c == '\'' || c == '$' || c == '-'
		|| c == '+' || c == '.' || c == '['
		|| c == ']' || c == '{' || c == '}'
		|| c == '(' || c == ')');
	/*return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r'
		|| c == '\'' || c == '$' || c == '-'
		|| c == '+' || c == '.');*/
}

static int is_stop_char(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r'
        || c == '\'' || c == '$' || c == '-'
		|| c == '+' || c == '.' || c == '['
		|| c == ']' || c == '{' || c == '}'
		|| c == '(' || c == ')' || c == '=');
}

int ft_strcmp_env(const char *s1, const char *s2)
{
	size_t i = 0;

	while (s1[i] && s2[i] && !is_stop_char(s1[i]) && !is_stop_char(s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if ((s1[i] == '\0' || is_stop_char(s1[i])) &&
		(s2[i] == '\0' || is_stop_char(s2[i])))
		return 0;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*int	ft_strcmp_env(const char *s1, const char *s2)
{
	/*while (*s1 && *s2)
    {
        if (*s1 == ' ' || *s1 == '\'' || *s2 == ' ' || *s2 == '\'')
            break;
        if (*s1 != *s2)
            return (unsigned char)*s1 - (unsigned char)*s2;
        s1++;
        s2++;
    }
	return (0);
    //return ((unsigned char)*(s1 - (unsigned char)*s2));
    size_t i = 0;

    while (s1[i] && s2[i] && s1[i] != ' ' && s2[i] != ' ') {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }

    // If we reached a space or null in both, check if both ended at the same time
    if ((s1[i] == '\0' || s1[i] == ' ') && (s2[i] == '\0' || s2[i] == ' '))
        return 0;

    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}*/

static size_t	min(size_t p, size_t q)
{
	if (p < q)
		return (p);
	return (q);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*ret_s;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		ret_s = (char *)malloc(sizeof(char));
		if (!ret_s)
			return (NULL);
		ret_s[0] = '\0';
		return (ret_s);
	}
	ret_s = (char *)malloc((min(len, ft_strlen(s) - start) + 1) * sizeof(char));
	if (!ret_s)
		return (NULL);
	while (i < len && *(s + start + i))
	{
		*(ret_s + i) = *(s + start + i);
		i++;
	}
	*(ret_s + i) = '\0';
	return (ret_s);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}
