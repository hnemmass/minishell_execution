/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:57:19 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/12 17:26:21 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

int		ft_strlen(const char *s);
char	*ft_strdup(char *s);
char	*ft_strndup(const char *s, int n);
void	free_minishell(t_minishell *s_minishell);
void	free_env(t_env *env);
int		ft_strcmp(const char *s1, const char *s2);

#endif