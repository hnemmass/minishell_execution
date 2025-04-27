/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:16:24 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/19 15:26:37 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_minishell	*init_minishell(char **env, t_status *e_status);
static t_status	minishell(t_minishell **s_minishell);
static void	print_env(t_env *env);
static void	print_tokens(t_token *s_tokens);
static char	*print_value(int v);
static void	print_cmds(t_cmd *s_cmd);

/*			---------		MAIN		--------			*/

int main(int arc, char **argv, char **env)
{
	t_minishell	*s_minishell;
	t_status	e_status;

	(void)arc;
	(void)argv;
	s_minishell = init_minishell(env, &e_status);
	if (!s_minishell)
		return (e_status);
	while (true)
	{
		e_status = minishell(&s_minishell);
		if (e_status == STATUS_EXIT_CMD)
			break ;
	}
	return (free_minishell(s_minishell), STATUS_SUCCESS);
}

static t_minishell	*init_minishell(char **env, t_status *e_status)
{
	t_minishell	*s_minishell;

	s_minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!s_minishell)
		return (*e_status = STATUS_MALLOC_FAIL, NULL);
	s_minishell->s_env = get_env(env);
	if (!s_minishell->s_env)
		return (*e_status = STATUS_MALLOC_FAIL, NULL);
	s_minishell->cwd = get_env_value(s_minishell->s_env, "PWD");
	if (!s_minishell->cwd)
		return (*e_status = STATUS_MALLOC_FAIL, free_env(s_minishell->s_env), NULL);
	s_minishell->stdfd[0] = dup(STDIN_FILENO);
	s_minishell->stdfd[1] = dup(STDOUT_FILENO);
	return (s_minishell);
}

static t_status	minishell(t_minishell **s_minishell)
{
	t_minishell	*s_ms;

	s_ms = *s_minishell;
	s_ms->cmdline = readline(PROMPT);
	if (!s_ms->cmdline)
		return (STATUS_FAILURE);
	if (ft_strcmp(s_ms->cmdline, "exit") == 0)
	{
		rl_clear_history();
		free(s_ms->cmdline);
		return (STATUS_EXIT_CMD);
	}
	add_history(s_ms->cmdline);
	s_ms->s_tokens = ft_tokenizer(s_ms->cmdline);
	s_ms->s_cmd = parse(s_ms->s_tokens);
	print_cmds(s_ms->s_cmd);
	//print_tokens(s_ms->s_tokens);
	free(s_ms->cmdline);
	ft_free_tokens(s_ms->s_tokens);
	return (STATUS_SUCCESS);
}


/*			--------		TESTING FUNCTIONS		--------			*/

static void	print_env(t_env *env)
{
	t_env	*node;

	node = env;
	while (node)
	{
		printf("%s=%s\n", node->name, node->value);
		node = node->next;
	}
}

static void	print_tokens(t_token *s_tokens)
{
	t_token	*result;

	result = s_tokens;
	printf("/*			******			*/\n");
	while(result)
	{
		printf("value = [%s]\n  type = %s\n", result->value, print_value(result->type));
		printf("---------------------------\n");
		result = result->next;
	}
	printf("/*			******			*/\n");
}

static char	*print_value(int v)
{
	char	*word = "word";
	char	*pipe = "pipe";
	char	*red_i = "red_in";
	char	*red_o = "red_out";
	char	*hdoc = "hdoc";
	char	*append = "append";

	if (v == 0)
		return (word);
	if (v == 1)
		return ("command");
	if (v == 2)
		return ("string");
	if (v == 3)
		return (pipe);
	if (v == 4)
		return (red_i);
	if (v == 5)
		return (red_o);
	if (v == 6)
		return (hdoc);
	if (v == 7)
		return ("EOF");
	if (v == 8)
		return (append);
	if (v == 9)
		return ("file");
	return ("NULL");
}

static void	print_cmds(t_cmd *head)
{
	while (head)
	{
		printf("Command type: %s\n", print_value(head->type));
		for (int i = 0; head->argv && head->argv[i]; i++)
			printf("  Arg[%d]: %s\n", i, head->argv[i]);
		head = head->next;
	}
}