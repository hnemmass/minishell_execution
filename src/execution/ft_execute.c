/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:45:12 by hnemmass          #+#    #+#             */
/*   Updated: 2025/05/11 16:40:17 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

static int	count_env_vars(t_env *env)
{
	t_env	*tmp_env;
	int		count;

	count = 0;
	tmp_env = env;
	while (tmp_env)
	{
		count++;
		tmp_env = tmp_env->next;
	}
	return (count);
}

static void	fill_env_array(char **env_array, t_env *env)
{
	t_env	*tmp_env;
	char	*temp;
	int		count;

	count = 0;
	tmp_env = env;
	while (tmp_env)
	{
		temp = ft_strjoin(tmp_env->name, "=");
		env_array[count] = ft_strjoin(temp, tmp_env->value);
		free(temp);
		count++;
		tmp_env = tmp_env->next;
	}
	env_array[count] = NULL;
}

char	**env_to_array(t_env *env)
{
	char	**env_array;
	int		count;

	count = count_env_vars(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	fill_env_array(env_array, env);
	return (env_array);
}

static int	exec_builtin(char **cmd, t_minishell *minishell)
{
	if (ft_strcmp(cmd[0], "cd") == 0)
		return (ft_cd(cmd, minishell->s_env, minishell));
	else if (ft_strcmp(cmd[0], "echo") == 0)
		return (ft_echo(cmd));
	else if (ft_strcmp(cmd[0], "env") == 0)
		return (ft_env(minishell->s_env));
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (ft_export(cmd, minishell->s_env));
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (ft_pwd(minishell));
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (ft_unset(cmd, minishell->s_env));
	else if (ft_strcmp(cmd[0], "exit") == 0)
		exit(0);
	return (1);
}

static void	setup_redirections(t_cmd *cmd, t_minishell *mini)
{
	t_redirect	*r;
	t_redirect	*checker;
	t_redirect *last_input;
	int			hdoc_count;
	int			hdoc_index;

	hdoc_count = 0;
	hdoc_index = 0;
	checker = cmd->s_redirect;
	while (checker)
	{
		if (checker->type == TOKEN_HDOC)
			hdoc_count++;
		if (checker->type == TOKEN_HDOC || checker->type == TOKEN_RED_IN)
			last_input = checker;
		checker = checker->next;
	}
	r = cmd->s_redirect;
	while (r)
	{
		if (r->type == TOKEN_HDOC)
		{
			apply_redirections(r, hdoc_count - hdoc_index - 1, mini, last_input);
			hdoc_index++;
		}
		else
			apply_redirections(r, 0, mini, last_input);
		r = r->next;
	}
}

static void	ft_handle_child(t_cmd *cmd, int prev_fd, int *pipe_fd, 
		int is_last, t_minishell *env)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (!is_last)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	setup_redirections(cmd, env);
	if (cmd->is_builtin)
	{
		env->exit_status = exec_builtin(cmd->argv, env);
		exit (env->exit_status);
	}
	else
		exec_cmd(cmd->argv, env->s_env);
	perror(cmd->argv[0]);
	exit(1);
}

static int	setup_pipe_and_fork(int *pipe_fd)
{
	pid_t	pid;

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}

static void	process_intermediate_cmds(t_cmd *cmd, t_minishell *env, 
		int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	pid = setup_pipe_and_fork(pipe_fd);
	if (pid == -1)
		return ;
	if (pid == 0)
		ft_handle_child(cmd, *prev_fd, pipe_fd, 0, env);
	if (*prev_fd != -1)
		close(*prev_fd);
	close(pipe_fd[1]);
	*prev_fd = pipe_fd[0];
}

static pid_t	handle_last_cmd(t_cmd *cmd, t_minishell *env, int prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		ft_handle_child(cmd, prev_fd, NULL, 1, env);
	return (pid);
}

static void	ft_execute_commands(t_cmd *cmd, t_minishell *env, 
		int prev_fd, pid_t *last_pid)
{
	int	i;

	i = 0;
	while (cmd->next)
	{
		if (ft_strcmp(cmd->argv[0], "./minishell") == 0)
		{
			if (i == 0)
				ft_putstr_fd("can't process minishell inside pipe\n", 1);
			i++;
			cmd = cmd->next;
			continue ;
		}
		process_intermediate_cmds(cmd, env, &prev_fd);
		cmd = cmd->next;
	}
	if (ft_strcmp(cmd->argv[0], "./minishell") == 0)
		return ;
	*last_pid = handle_last_cmd(cmd, env, prev_fd);
	if (prev_fd != -1)
		close(prev_fd);
}

static pid_t	execute_single_cmd(t_cmd *cmd, t_minishell *env)
{
	pid_t	pid;
	int		exit_status;

	if (cmd->is_builtin)
	{
		int fd1 = dup(STDIN_FILENO);
		int fd2 = dup(STDOUT_FILENO);
		setup_redirections(cmd, env);
		env->exit_status = exec_builtin(cmd->argv, env);
		dup2(fd1, STDIN_FILENO);
		dup2(fd2, STDOUT_FILENO);
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		ft_handle_child(cmd, -1, NULL, 1, env);
	return (pid);
}

void	ft_execute(t_cmd *cmd_list, t_minishell *env)
{
	int		prev_fd;
	pid_t	last_pid;
	int		status;
	pid_t	pid;

	prev_fd = -1;
	last_pid = -1;
	if (!cmd_list->next)
		last_pid = execute_single_cmd(cmd_list, env);
	else
		ft_execute_commands(cmd_list, env, prev_fd, &last_pid);
	if (last_pid != -1)
	{
		while ((pid = wait(&status)) != -1)
		{
			if (pid == last_pid)
				env->exit_status = WEXITSTATUS(status);
		}
	}
}