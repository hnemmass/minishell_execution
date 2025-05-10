/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:50:56 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/03 18:58:44 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_arg(t_tokens_type type)
{
	return (type == TOKEN_CMD || type == TOKEN_ARG || type == TOKEN_WORD);
}

int	is_redirection(t_tokens_type type)
{
	return (type == TOKEN_APPEND || type == TOKEN_HDOC || type == TOKEN_RED_IN
		|| type == TOKEN_RED_OUT);
}

int	is_file_eof(t_tokens_type type)
{
	return (type == TOKEN_FILE || type == TOKEN_EOF);
}
