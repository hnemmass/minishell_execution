/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:12:57 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/11 17:01:48 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void		free_env(t_env *env);
static void	free_argv(char **argv);
static void	free_redirection(t_redirect *s_redirection);

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

void free_commands(t_minishell *s_minishell)
{
    t_cmd *current;
    t_cmd *next;

    if (!s_minishell->s_cmd)
        return;
    
    current = s_minishell->s_cmd;
    while (current)
    {
        next = current->next;  // Save next pointer before freeing current
        
        if (current->argv != NULL)
            free_argv(current->argv);
        if (current->s_redirect != NULL)
            free_redirection(current->s_redirect);
        
        free(current);
        current = next;  // Move to next node using our saved pointer
    }
    
    s_minishell->s_cmd = NULL;  // Prevent dangling pointer
}

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return;
	while (argv[i])
		free(argv[i++]);
}

static void	free_redirection(t_redirect *s_redirection)
{
	t_redirect	*node;
	t_redirect	*cup;

	node = s_redirection;
	cup = NULL;
	while (node)
	{
		cup = node->next;
		free(node->file);
		free(node);
		node = cup;
	}
}