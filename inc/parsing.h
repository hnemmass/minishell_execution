/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 19:32:07 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/09 18:18:02 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "structs.h"

t_status	parse_command_line(t_minishell *s_minishell);
void		handle_quotes(t_token *s_tokens, t_env *s_env, int	exit_status);
int			has_var(char *s);
int			has_quotes(char *s);
t_status	check_syntax(t_token *s_tokens);
void		expand_variables(t_substring **head, t_env *s_env, int exit_status);
char		*scan_string(char *s, t_env *s_env, int exit_status);
char		*expand_split(char *s, t_env *s_env);
void		expand_unquoted(t_token *s_tokens, t_env *s_env);

#endif