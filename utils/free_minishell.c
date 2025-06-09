/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:12:57 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/23 17:28:03 by yhajbi           ###   ########.fr       */
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

void	free_commands(t_minishell *s_minishell)
{
	t_cmd	*node;
	t_cmd	*cup;

	if (!s_minishell->s_cmd)
		return ;
	node = s_minishell->s_cmd;
	cup = NULL;
	while (node)
	{
		cup = node->next;
		free_argv(node->argv);
		free_redirection(node->s_redirect);
		free(node);
		node = cup;
	}
}

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
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