/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:16:34 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/19 15:26:48 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void		process_tokens(t_token *s_tokens);
static t_cmd	*parse_processed_tokens(t_token *s_tokens);
static	void	cmdadd_back(t_cmd **head, char **argv, t_tokens_type type);
static char		**make_argv(t_token *cur, t_token *prv);
static char	**make_argv_helper(char *s);
static int		count_args(t_token *s_tokens);
static int		identify_cmd(char *cmd);

t_cmd	*parse(t_token *s_tokens)
{
	t_cmd	*s_cmd;

	process_tokens(s_tokens);
	s_cmd = parse_processed_tokens(s_tokens);
	return (s_cmd);
}

static void		process_tokens(t_token *s_tokens)
{
	t_token	*node;
	t_token	*prv;

	node = s_tokens;
	prv = s_tokens;
	while (node)
	{
		if (node->type == TOKEN_WORD && (prv->type == TOKEN_RED_IN || prv->type == TOKEN_RED_OUT))
			node->type = TOKEN_FILE;
		prv = node;
		node = node->next;
	}
}

static t_cmd	*parse_processed_tokens(t_token *s_tokens)
{
	t_cmd	*s_cmd;
	t_token	*node;
	t_token	*prv;

	s_cmd = NULL;
	node = s_tokens;
	prv = s_tokens;
	while (node)
	{
		if (node->type == TOKEN_HDOC)
			cmdadd_back(&s_cmd, make_argv(node, prv), node->type);
		else if (prv->type == TOKEN_HDOC && node->type == TOKEN_WORD)
			cmdadd_back(&s_cmd, make_argv(node, prv), TOKEN_EOF);
		else if (node->type == TOKEN_WORD && prv->type != TOKEN_WORD)
			cmdadd_back(&s_cmd, make_argv(node, prv), TOKEN_CMD);
		else if (node->type == TOKEN_FILE)
			cmdadd_back(&s_cmd, make_argv(node, prv), TOKEN_FILE);
		prv = node;
		node = node->next;
	}
	return (s_cmd);
}

static	void	cmdadd_back(t_cmd **head, char **argv, t_tokens_type type)
{
	    t_cmd *new;
    t_cmd *node;

    new = malloc(sizeof(t_cmd));
    if (!new)
        return;
    new->argv = argv;
    new->type = type;
    new->is_builtin = identify_cmd(argv[0]);
    new->next = NULL;
    if (*head == NULL)
    {
        *head = new;
        return;
    }
    node = *head;
    while (node->next)
        node = node->next;
    node->next = new;
}

static char	**make_argv(t_token *cur, t_token *prv)
{
	char	**argv;
	t_token	*node;
	int		cmd_sz;
	int		i;

	node = cur;
	if (node->type == TOKEN_HDOC)
		return (make_argv_helper("here_doc"));
	if (node->type == TOKEN_WORD && prv->type == TOKEN_HDOC)
		return (make_argv_helper(node->value));
	if (node->type == TOKEN_FILE)
		return (make_argv_helper(node->value));
	cmd_sz = count_args(node);
	i = 0;
	argv = malloc((cmd_sz + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	while (node && node->type == TOKEN_WORD)
	{
		argv[i] = ft_strdup(node->value);
		node = node->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

static char	**make_argv_helper(char *s)
{
	char	**ret;

	ret = malloc(2 * sizeof(char *));
	ret[0] = ft_strdup(s);
	ret[1] = NULL;
	return (ret);
}

static int	count_args(t_token *s_tokens)
{
	int		cnt;
	t_token	*node;

	cnt = 0;
	node = s_tokens;
	while (node && node->type == TOKEN_WORD)
	{
		cnt++;
		node = node->next;
	}
	return (cnt);
}

static int	identify_cmd(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}