/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:57:19 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/09 18:20:02 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

int		ft_strlen(const char *s);
char	*ft_strdup(char *s);
char	*ft_strndup(const char *s, int n);
void	free_minishell(t_minishell *s_minishell);
void	free_env(t_env *env);
void	free_commands(t_minishell *s_minishell);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, const char *delimiters);
size_t	ft_strlcpy(char *dest, const char *src, size_t n);
char	*ft_strconcat(const char *s1, const char *s2);
int		ft_strcmp_env(const char *s1, const char *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	ft_strcpy(char *dest, const char *src);
void	*ft_calloc(size_t count, size_t size);
int		is_arg(t_tokens_type type);
int		is_redirection(t_tokens_type type);
int		is_file_eof(t_tokens_type type);
char	*ft_strchr(const char *s, int c);
char	*ft_itoa(int n);

#endif