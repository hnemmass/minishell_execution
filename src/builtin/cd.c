/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:16:38 by hnemmass          #+#    #+#             */
/*   Updated: 2025/05/08 14:35:13 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

static char	*search_for_dir(t_env *env, char *name)
{
	t_env	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		if (!ft_strcmp(tmp_env->name, name))
			return (tmp_env->value);
		tmp_env = tmp_env->next;
	}
	return (NULL);
}

static void	update_oldpwd(t_env *env, char *old_pwd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "OLDPWD"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(old_pwd);
			break ;
		}
		tmp = tmp->next;
	}
}

static void	update_pwd(t_env *env, char *cwd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PWD"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(cwd);
			break ;
		}
		tmp = tmp->next;
	}
}

static int	change_pwd(char *path, t_env *env, t_minishell *mini)
{
	char	*cwd;
	char	*old_pwd;
	char	*b_old_pwd;

	b_old_pwd = NULL;
	old_pwd = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		printf("cd: error retrieving current directory: getcwd: ");
		printf("cannot access parent directories: No such file or directory\n");
		if (!search_for_dir(env, "PWD"))
		{
			b_old_pwd = ft_strdup(mini->cwd);
			cwd = ft_strjoin(mini->cwd, "/");
			cwd = ft_strjoin(cwd, path);
		}
		else
		{
			cwd = ft_strjoin(search_for_dir(env, "PWD"), "/");
			cwd = ft_strjoin(cwd, path);
		}
	}
	old_pwd = search_for_dir(env, "PWD");
	if (!old_pwd)
	{
		old_pwd = search_for_dir(env, "OLDPWD");
		if (old_pwd)
			update_oldpwd(env, mini->cwd);
	}
	else if (old_pwd)
	{
		b_old_pwd = search_for_dir(env, "OLDPWD");
		if (b_old_pwd)
			update_oldpwd(env, old_pwd);
		update_pwd(env, cwd);
	}
	free(mini->cwd);
	mini->cwd = ft_strdup(cwd);
	free(cwd);
	return (0);
}

int	ft_cd(char **cmd, t_env *env, t_minishell *mini)
{
	char	*path;

	if (!env)
		return (printf("environment not found\n"), 1);
	if (cmd[1] && cmd[2])
		return (printf("cd: too many arguments\n"), 1);
	if (!cmd[1])
	{
		path = search_for_dir(env, "HOME");
		if (!path)
			return (printf("cd: HOME not set\n"), 1);
	}
	else
		path = cmd[1];
	if (chdir(path) == -1)
		return (perror("cd"), 1);
	if (change_pwd(path, env, mini))
		return (1);
	return (0);
}
