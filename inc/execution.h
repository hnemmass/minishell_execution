/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:11:28 by hnemmass          #+#    #+#             */
/*   Updated: 2025/05/08 15:40:21 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "structs.h"
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char	**env_to_array(t_env *env);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strjoin_free(char *s1, char *s2);
int		ft_strcmp(const char *s1, const char *s2);
void	free_split_2(char **split);
char	*get_next_line(int fd);
char	**ft_split_2(char const *s, char c);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strdup(char *s);
int		apply_redirections(t_redirect *red, int i);
int		ft_cd(char **cmd, t_env *env, t_minishell *mini);
int		ft_echo(char **cmd);
int		ft_env(t_env *env);
int		ft_export(char **cmd, t_env *env);
int		ft_pwd(t_minishell *mini);
int		ft_unset(char **cmd, t_env *env);
void	ft_execute(t_cmd *data, t_minishell *env);
void	exec_cmd(char **cmd, t_env *env);
char	*make_path(t_env *env);

#endif