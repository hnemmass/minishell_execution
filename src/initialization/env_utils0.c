/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:07:45 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/12 15:12:55 by yhajbi           ###   ########.fr       */
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
}