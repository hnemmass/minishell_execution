/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:50:57 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/09 18:19:52 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	if (!src || !dest)
		return;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
}

static int	ft_countlen(int n)
{
	int	count;

	count = 0;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	ft_fillarr(char *index, int n)
{
	if (n <= 9)
		*index = n + 48;
	else
	{
		*index = n % 10 + 48;
		ft_fillarr(index - 1, n / 10);
	}
}

char	*ft_itoa(int n)
{
	char	*arr;
	int		len;
	int		neg;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	len = ft_countlen(n);
	neg = 0;
	if (n < 0)
	{
		neg = 1;
		n *= -1;
		len++;
	}
	arr = (char *)malloc((len + 1) * sizeof(char));
	if (!arr)
		return (NULL);
	ft_fillarr(arr + len - 1, n);
	if (neg)
		*arr = '-';
	*(arr + len) = '\0';
	return (arr);
}
