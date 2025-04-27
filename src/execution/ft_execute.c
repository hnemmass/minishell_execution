/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:45:12 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/27 16:03:08 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

char	**env_to_array(t_env *env)
{
	t_env	*tmp_env;
	char	**env_array;
	char	*temp;
	int		count;

	count = 0;
	tmp_env = env;
	while(tmp_env)
	{
		count++;
		tmp_env = tmp_env->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (env_array)
		return (NULL);
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
	return (env_array);
}

static int	exec_builtin(char **cmd, t_env *env)
{
	if (ft_strcmp(cmd[0], "cd") == 0)
		ft_cd(cmd, env);
	else if (ft_strcmp(cmd[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd[0], "env") == 0)
		ft_env(env);
	else if (ft_strcmp(cmd[0], "export") == 0)
		ft_export(cmd, env);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd[0], "unset") == 0)
		ft_unset(cmd, env);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		exit (0);
	return (1);
}

static void	ft_handle_child(t_cmd *cmd, int prev_fd, int *pipe_fd, int is_last, t_minishell *env)
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
    for (t_redirect *r = cmd->s_redirect; r; r = r->next)
        apply_redirections(r);
    if (cmd->is_builtin)
        exec_builtin(cmd->argv, env->s_env);
    else
        exec_cmd(cmd->argv, env->s_env);
    perror(cmd->argv[0]);
    _exit(1);
}

static void	ft_execute_commands(t_cmd *cmd, t_minishell *env, int prev_fd, pid_t *last_pid)
{
    int   pipe_fd[2];
    int   is_last;
    pid_t pid;

    while (cmd->next)
    {
        if (pipe(pipe_fd) < 0)
        {
            perror("pipe");
            return;
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return;
        }
        if (pid == 0)
            ft_handle_child(cmd, prev_fd, pipe_fd, 0, env);
        if (prev_fd != -1)
            close(prev_fd);
        close(pipe_fd[1]);
        prev_fd = pipe_fd[0];
        cmd = cmd->next;
    }    
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
        ft_handle_child(cmd, prev_fd, NULL, 1, env);
    *last_pid = pid;
    if (prev_fd != -1)
        close(prev_fd);
}

void	ft_execute(t_cmd *cmd_list, t_minishell *env)
{
    int     prev_fd = -1;
    pid_t   last_pid = -1;
    int     status;
    pid_t   pid;
    static int exit_status = 0;

    if (!cmd_list->next)
    {
        if (cmd_list->is_builtin)
        {
            for (t_redirect *r = cmd_list->s_redirect; r; r = r->next)
                apply_redirections(r);
            exec_builtin(cmd_list->argv, env->s_env);
            return;
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return;
        }
        if (pid == 0)
            ft_handle_child(cmd_list, -1, NULL, 1, env);
        last_pid = pid;
    }
    else
        ft_execute_commands(cmd_list, env, prev_fd, &last_pid);
    while ((pid = wait(&status)) != -1)
    {
        if (pid == last_pid)
            exit_status = WEXITSTATUS(status);
    }
}
