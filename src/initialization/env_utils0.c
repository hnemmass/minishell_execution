/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:07:45 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/01 17:50:41 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_env_value(t_env *s_env, char *name)
{
	t_env	*node;

	node = s_env;
	while (node)
	{
		if (ft_strcmp(node->name, name) == 0)
			return (ft_strdup(node->value));
		node = node->next;
	}
	return (NULL);
}

char	*get_env_value2(t_env *s_env, char *name)
{
	int		i;
	t_env	*node;

	i = 0;
	node = s_env;
	while (name[i] == '$')
		i++;
	while (node)
	{
		if (ft_strcmp_env(node->name, name + i) == 0)
			return (node->value);
		node = node->next;
	}
	return (NULL);
}

int	has_var(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (1);
		i++;
	}
	return (0);
}
