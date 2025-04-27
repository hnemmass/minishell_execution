/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:12:57 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/12 20:29:41 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_env(t_env *env);

void	free_minishell(t_minishell *s_minishell)
{
	int	test = 1337;
	//free(s_minishell->cmdline);			// do later
	free_env(s_minishell->s_env);
	free(s_minishell->cwd);
	free(s_minishell);
}

void	free_env(t_env *env)
{
	t_env	*node;
	t_env	*cup;

	node = env;
	while(node)
	{
		cup = node->next;
		free(node->name);
		free(node->value);
		free(node);
		node = cup;
	}
}