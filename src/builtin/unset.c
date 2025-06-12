/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:48:33 by hnemmass          #+#    #+#             */
/*   Updated: 2025/06/11 16:28:49 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

int	ft_unset(char **cmd, t_env **env)
{
	int		i;
	t_env	*current;
	t_env	*prev;

	i = 1;
	if (*env == NULL)
		return (0);
	while (cmd[i])
	{
		current = *env;
		prev = NULL;
		if (cmd[i][0] == '_' && !cmd[i][1])
		{
			i++;
			continue ;
		}
		while (current && strcmp(current->name, cmd[i]) != 0)
		{
			prev = current;
			current = current->next;
		}
		if (current)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->name);
			free(current->value);
			free(current);
		}
		i++;
	}
	return (0);
}
