/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:08:40 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/23 16:40:00 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void			process_tokens(t_token *s_tokens);
t_cmd				*parse_tokens(t_token *s_tokens);
static t_cmd		*create_cmd(void);
static void			add_cmd(t_cmd **head, t_cmd *new);
static t_redirect	*create_redirect(t_tokens_type new_type, char *new_file);
static void			add_redirect(t_redirect **head, t_redirect *new);
static void			add_arg(t_cmd **head, char *arg);
static int			is_builtin(char *cmd);
static void			free_argv(char **argv);
static char			*expand_env_var(t_env *s_env, char *var);

t_status	parse_command_line(t_minishell *s_minishell)
{
	if (!s_minishell || ft_strcmp("", s_minishell->cmdline) == 0)
		return (STATUS_FAILURE);
	if (!(s_minishell->s_tokens))
		return (printf("minishell: syntax error\n"), STATUS_FAILURE);
	if (check_syntax(s_minishell->s_tokens) == STATUS_SYNTAX_ERR)
		return (printf("minishell: syntax error\n"), STATUS_FAILURE);
	process_tokens(s_minishell->s_tokens);
	handle_quotes(s_minishell->s_tokens, s_minishell->s_env, s_minishell->exit_status);
	//expand_unquoted(s_minishell->s_tokens, s_minishell->s_env);
	s_minishell->s_cmd = parse_tokens(s_minishell->s_tokens);
	if (!s_minishell->s_cmd)
		return (STATUS_FAILURE);
	return (STATUS_SUCCESS);
}

/*void	expand_unquoted(t_token *s_tokens, t_env *s_env)
{
	t_token	*node;
	char	*old_value;

	node = s_tokens;
	old_value = NULL;
	while (node)
	{
		old_value = node->value;
		if (has_var(node->value) == 1 && has_quotes(node->value) == 0)
		{
			node->value = expand_split(node->value, s_env);
			if (old_value != NULL)
				free(old_value);
		}
		node = node->next;
	}
}*/

static void		process_tokens(t_token *s_tokens)
{
	char	c;
	t_token	*node;
	t_token	*prv;

	node = s_tokens;
	prv = s_tokens;
	while (node)
	{
		c = *(node->value);
		//if (c == '$')
		//	node->type = TOKEN_VAR;
		if (node->type == TOKEN_WORD && (prv->type == TOKEN_RED_IN || prv->type == TOKEN_RED_OUT))
			node->type = TOKEN_FILE;
		else if (node->type == TOKEN_WORD && prv->type == TOKEN_HDOC)
			node->type = TOKEN_EOF;
		else if (node->type == TOKEN_WORD && prv->type == TOKEN_APPEND)
			node->type = TOKEN_FILE;
		else if (node->type == TOKEN_WORD && (prv->type != TOKEN_WORD
			&& prv->type != TOKEN_CMD && prv->type != TOKEN_ARG))
			node->type = TOKEN_CMD;
		else if (node->type == TOKEN_WORD && (prv->type == TOKEN_CMD || prv->type == TOKEN_ARG))
			node->type = TOKEN_ARG;
		prv = node;
		node = node->next;
	}
}

t_cmd	*parse_tokens(t_token *s_tokens)
{
	int		first;
	t_cmd	*s_cmd;
	t_cmd	*curr_cmd;
	t_token	*curr_token;

	//s_cmd = malloc(sizeof(s_cmd));
	first = 1;
	s_cmd = create_cmd();
	if (!s_cmd)
		return (NULL);
	curr_cmd = s_cmd;
	curr_token = s_tokens;
	while (curr_token)
	{
		if (curr_token->type == TOKEN_PIPE)
		{
			if (first == 1)
			{
				curr_cmd = create_cmd();
				first = 0;
			}
			else
			{
				add_cmd(&s_cmd, curr_cmd);
				curr_cmd = create_cmd();
			}
		}
		else if (is_arg(curr_token->type))
			add_arg(&curr_cmd, curr_token->value);
		else if (is_redirection(curr_token->type) && is_file_eof(curr_token->next->type))
			add_redirect(&(curr_cmd->s_redirect), create_redirect(curr_token->type, curr_token->next->value));
		curr_token = curr_token->next;
	}
	if (!first)
		add_cmd(&s_cmd, curr_cmd);
	return (s_cmd);
}

static t_cmd	*create_cmd(void)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = NULL;
	new->is_builtin = 0;
	new->s_redirect = NULL;
	new->next = NULL;
	return (new);
}

static void	add_cmd(t_cmd **head, t_cmd *new)
{
	t_cmd	*node;

	node = *head;
	if (!node)
	{
		*head = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
}

static t_redirect	*create_redirect(t_tokens_type new_type, char *new_file)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (!new)
		return (NULL);
	new->type = new_type;
	new->file = ft_strdup(new_file);
	new->heredoc_fd = -1;
	new->next = NULL;
	return (new);
}

static void	add_redirect(t_redirect **head, t_redirect *new)
{
	t_redirect	*node;

	node = *head;
	if (!*head)
	{
		*head = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
}

static void	add_arg(t_cmd **head, char *arg)
{
	t_cmd	*s_cmd;
	char	**new_argv;
	int		i;
	int		j;

	s_cmd = *head;
	if (!arg)
		return ;
	if (!s_cmd->argv)
	{
		new_argv = malloc(sizeof(char *) * 2);
		if (!new_argv)
			return ;
		new_argv[0] = ft_strdup(arg);
		new_argv[1] = NULL;
		s_cmd->argv = new_argv;
		s_cmd->is_builtin = is_builtin(s_cmd->argv[0]);
		return ;
	}
	i = 0;
	while (s_cmd->argv[i])
		i++;
	new_argv = malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		return ;
	j = 0;
	while (j < i)
	{
		new_argv[j] = ft_strdup(s_cmd->argv[j]);
		j++;
	}
	new_argv[j] = ft_strdup(arg);
	new_argv[j + 1] = NULL;
	s_cmd->is_builtin = is_builtin(new_argv[0]);
	free_argv(s_cmd->argv);
	s_cmd->argv = new_argv;
}

static int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		free(argv[i++]);
	//free(argv);
}

static char	*expand_env_var(t_env *s_env, char *var)
{
	int		i;
	t_env	*node;

	i = 0;
	node = s_env;
	while (var[i] && var[i] == '$')
		i++;
	while (node)
	{
		if (ft_strcmp(var + i, node->name) == 0)
			return (ft_strdup(node->value));
		node = node->next;
	}
	return (ft_strdup(""));
}