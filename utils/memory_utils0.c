/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:39:44 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/01 17:06:58 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	ft_bzero(void *p, size_t n);

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;
	int		total;

	total = count * size;
	p = malloc(total);
	if (!p)
		return (NULL);
	ft_bzero(p, total);
	return (p);
}

static void	ft_bzero(void *p, size_t n)
{
	unsigned char	*s;
	size_t				i;

	s = (unsigned char *)p;
	i = 0;
	while (i < n)
		s[i++] = 0;
}