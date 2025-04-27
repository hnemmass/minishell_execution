/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:16:38 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/23 19:04:50 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// typedef struct s_env {
// 	char *name;
// 	char *value;
// 	struct s_env *next;
// } t_env;

// // ===== Declare your functions here (from cd.c) =====
// int cd(char **cmd, struct s_env *env);
// char *ft_strdup(const char *s);
// int ft_strcmp(const char *s1, const char *s2);

// // ===== Basic strdup and strcmp =====
// char *ft_strdup(const char *s)
// {
// 	char *copy = malloc(strlen(s) + 1);
// 	if (!copy)
// 		return (NULL);
// 	strcpy(copy, s);
// 	return copy;
// }

// int ft_strcmp(const char *s1, const char *s2)
// {
// 	while (*s1 && *s2 && *s1 == *s2)
// 	{
// 		s1++;
// 		s2++;
// 	}
// 	return (*(unsigned char *)s1 - *(unsigned char *)s2);
// }

// // ===== Helper to print environment =====
// void print_env(t_env *env)
// {
// 	while (env)
// 	{
// 		printf("%s=%s\n", env->name, env->value);
// 		env = env->next;
// 	}
// }

// // ===== Helper to create env variable =====
// t_env *create_env(char *name, char *value)
// {
// 	t_env *new = malloc(sizeof(t_env));
// 	if (!new)
// 		return NULL;
// 	new->name = ft_strdup(name);
// 	new->value = ft_strdup(value);
// 	new->next = NULL;
// 	return new;
// }

static char	*search_for_dir(t_env *env, char *name)
{
	t_env	*tmp_env;

	tmp_env = env;
	while(tmp_env)
	{
		if (!ft_strcmp(tmp_env->name, name))
			return (tmp_env->value);
		tmp_env = tmp_env->next;
	}
	return (NULL);
}
static int	change_pwd(char *path, t_env *env)
{
	t_env	*tmp;
	char	*cwd;
	char	*old_pwd;
	int exitstatus =0;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		exitstatus = 1;
		perror("getcwd:");
	}
	old_pwd = search_for_dir(env, "PWD");
	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "OLDPWD"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(old_pwd);
			break;
		}
		tmp = tmp->next;
	}
	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PWD"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(cwd);
			break;
		}
		tmp = tmp->next;
	}
	free (cwd);
	return (exitstatus);
}

int	ft_cd(char **cmd, t_env *env)
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
	if (change_pwd(path, env))
		return (perror("cd"), 1);
	return (0);
}

// // ===== Main for testing cd =====
// int main(int argc, char **argv)
// {
// 	t_env *env = create_env("PWD", getcwd(NULL, 0));
// 	t_env *oldpwd = create_env("OLDPWD", "");
// 	t_env *home = create_env("HOME", getenv("HOME"));

// 	// Link env list
// 	env->next = oldpwd;
// 	oldpwd->next = home;

// 	printf("Before cd:\n");
// 	print_env(env);

// 	// Simulate cd
// 	cd(&argv[1], env);


// 	printf("\nAfter cd:\n");
// 	print_env(env);

// 	// Free environment (basic cleanup)
// 	t_env *tmp;
// 	while (env)
// 	{
// 		tmp = env->next;
// 		free(env->name);
// 		free(env->value);
// 		free(env);
// 		env = tmp;
// 	}
// 	return 0;
// }