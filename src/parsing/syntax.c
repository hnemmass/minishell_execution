/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:10:32 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/24 17:32:47 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_redirection(t_token **s_tokens);
static int	check_append_hdoc(t_token **s_tokens);
static int	check_pipe(t_token **s_tokens);
static int	check_quotes(t_token **s_tokens);

t_status	check_syntax(t_token *s_tokens)
{
	int		i;
	int		ret;
	t_token	*node;

	i = 0;
	ret = 1;
	node = s_tokens;
	while (node)
	{
		if (node->type == TOKEN_RED_IN || node->type == TOKEN_RED_OUT)
			ret = check_redirection(&node);
		else if (node->type == TOKEN_APPEND || node->type == TOKEN_HDOC)
			ret = check_append_hdoc(&node);
		else if (node->type == TOKEN_PIPE)
			ret = check_pipe(&node);
		else if (node->type == TOKEN_WORD)
			ret = check_quotes(&node);
		if (ret == 0)
			break ;
		node = node->next;
	}
	if (ret == 0)
		return (STATUS_SYNTAX_ERR);
	return (STATUS_SUCCESS);
}

static int	check_redirection(t_token **s_tokens)
{
	t_token	*next;
	t_token	*node;

	next = (*s_tokens)->next;
	node = (*s_tokens);
	if (next == NULL)
		return (0);
	if (next->type == node->type)
		return (0);
	if ((next->type == TOKEN_RED_IN || next->type == TOKEN_RED_OUT)
		&& (node->type == TOKEN_RED_IN || node->type == TOKEN_RED_OUT))
		return (0);
	if (next->type == TOKEN_APPEND || next->type == TOKEN_HDOC)
		return (0);
	return (1);
}

static int	check_append_hdoc(t_token **s_tokens)
{
	t_token	*node;
	t_token	*next;

	node = *s_tokens;
	next = (*s_tokens)->next;
	if (next == NULL)
		return (0);
	if (next->type != TOKEN_WORD)
		return (0);
	return (1);
}

static int	check_pipe(t_token **s_tokens)
{
	t_token	*node;
	t_token	*next;

	node = *s_tokens;
	next = (*s_tokens)->next;
	if (next == NULL)
		return (0);
	if (next->type == TOKEN_PIPE)
		return (0);
	return (1);
}

static int	check_quotes(t_token **s_tokens)
{
	char	*s;
	char	cur_quote;
	int		i;

	s = (*s_tokens)->value;
	cur_quote = '\0';
	i = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
			cur_quote = s[i];
		else if (s[i] == cur_quote)
			cur_quote = '\0';
		i++;
	}
	if (cur_quote != '\0')
		return (0);
	return (1);
}