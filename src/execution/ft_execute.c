/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:45:12 by hnemmass          #+#    #+#             */
/*   Updated: 2025/06/12 15:34:57 by hnemmass         ###   ########.fr       */
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
		return (ft_unset(cmd, &minishell->s_env));
	else if (ft_strcmp(cmd[0], "exit") == 0)
		return (ft_exit(cmd, minishell));
	return (1);
}

static int	total_alloc(int n)
{
	int	j;
	int	k;

	k = 0;
	if (n <= 0)
		k = 1;
	j = n;
	while (j != 0)
	{
		j = j / 10;
		k++;
	}
	return (k);
}

char	*ft_itoa2(int nb)
{
	char	*s;
	int		k;
	long	n;	

	n = nb;
	k = total_alloc(n);
	s = malloc(k + 1);
	if (!s)
		return (NULL);
	s[k] = '\0';
	if (n == 0)
		s[0] = '0';
	else if (n < 0)
	{
		s[0] = '-';
		n = -n;
	}
	while (n != 0)
	{
		s[k - 1] = (n % 10) + '0';
		n /= 10;
		k--;
	}
	return (s);
}

static int	compare(char *line, char *delimiter)
{
	int	i;

	i = 0;
	if (!line && !delimiter)
		return (1);
	while (line[i] && delimiter[i] && line[i] == delimiter[i])
		i++;
	if (line[i] == '\0' && !delimiter[i])
		return (0);
	return (1);
}

static int	append_itoa(char **str, int num)
{
	char	*num_str;
	char	*new_str;

	num_str = ft_itoa2(num);
	if (!num_str)
		return (0);
	new_str = ft_strjoin(*str, num_str);
	free(num_str);
	free(*str);
	*str = new_str;
	return (new_str != NULL);
}

static char	*create_heredoc_filename(int cmd_index, int hdoc_index)
{
	char	*filename;
	char	*tmp;
	pid_t	pid;

	pid = getpid();
	filename = ft_strdup("/tmp/heredoc_tmp_");
	if (!filename || !append_itoa(&filename, cmd_index))
		return (free(filename), NULL);
	tmp = ft_strjoin(filename, "_");
	free(filename);
	if (!tmp || !append_itoa(&tmp, hdoc_index))
		return (free(tmp), NULL);
	filename = ft_strjoin(tmp, "_");
	free(tmp);
	if (!filename || !append_itoa(&filename, pid))
		return (free(filename), NULL);
	return (filename);
}

static int	open_heredoc_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		perror(filename);
	unlink(filename);
	return (fd);
}

void close_fd(int fd, int flag)
{
	static char temp_fd;

	if (flag == 1)
		close(temp_fd);
	else
		temp_fd = fd;
}

static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		close_fd(0, 1);
		signal(SIGINT, SIG_DFL);
		kill(getpid(), SIGINT);
	}
}

static void	setup_heredoc_signals(int temp_fd)
{
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

static int	heredoc_child_process(char *delimiter, char *filename, t_minishell *mini)
{
	char	*line;
	int		temp_fd;

	temp_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	close_fd(temp_fd, 0);
	if (temp_fd == -1)
	{
		perror("heredoc");
		exit(1);
	}
	setup_heredoc_signals(temp_fd);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (!compare(line, delimiter))
		{
			free(line);
			break;
		}
		line = scan_string(line, mini->s_env, mini->exit_status);
		line = ft_strjoin_free(line, "\n");
		ft_putstr_fd(line, temp_fd);
		free(line);
	}
	close(temp_fd);
	exit(0);
}

static int	handle_heredoc_result(int status, char *filename, t_minishell *mini)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			unlink(filename);
			free(filename);
			mini->exit_status = 130;
			return (-1);
		}
	}
	return (open_heredoc_file(filename));
}

static int	process_heredoc(char *delimiter, int cmd_index, 
							int hdoc_index, t_minishell *mini)
{
	char	*filename;
	pid_t	pid;
	int		status;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	filename = create_heredoc_filename(cmd_index, hdoc_index);
	if (!filename)
		return (-1);
	old_sigint = signal(SIGINT, SIG_IGN);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), free(filename), signal(SIGINT, old_sigint)
				, signal(SIGQUIT, old_sigquit), -1);
	if (pid == 0)
		heredoc_child_process(delimiter, filename, mini);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	return (handle_heredoc_result(status, filename, mini));
}

static t_redirect	*find_last_heredoc(t_redirect *redir)
{
	t_redirect	*last;

	last = NULL;
	while (redir)
	{
		if (redir->type == TOKEN_HDOC)
			last = redir;
		redir = redir->next;
	}
	return (last);
}

static int	process_cmd_heredocs(t_cmd *cmd, int c_idx, t_minishell *mini)
{
	t_redirect	*redir;
	t_redirect	*last_hdoc;
	int			h_idx;

	last_hdoc = find_last_heredoc(cmd->s_redirect);
	h_idx = 0;
	redir = cmd->s_redirect;
	while (redir)
	{
		if (redir->type == TOKEN_HDOC)
		{
			redir->heredoc_fd = process_heredoc(redir->file, c_idx, h_idx, mini);
			if (redir->heredoc_fd == -1)
				return (-1);
			if (redir != last_hdoc)
			{
				close(redir->heredoc_fd);
				redir->heredoc_fd = -1;
			}
			h_idx++;
		}
		redir = redir->next;
	}
	return (0);
}

static int	process_heredocs(t_cmd *cmd_list, t_minishell *mini)
{
	t_cmd	*cmd;
	int		c_idx;

	c_idx = 0;
	cmd = cmd_list;
	while (cmd)
	{
		if (process_cmd_heredocs(cmd, c_idx, mini) == -1)
			return (-1);
		cmd = cmd->next;
		c_idx++;
	}
	return (0);
}

static void	close_heredocs(t_cmd *cmd_list)
{
	t_cmd		*cmd;
	t_redirect	*r;
	
	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->s_redirect;
		while (r)
		{
			if (r->type == TOKEN_HDOC && r->heredoc_fd >= 0)
			{
				close(r->heredoc_fd);
				r->heredoc_fd = -1;
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
}

static void	setup_redirections(t_cmd *cmd, t_minishell *mini)
{
	t_redirect	*r;

	r = cmd->s_redirect;
	while (r)
	{
		if (r->type == TOKEN_HDOC)
		{
			if (r->heredoc_fd >= 0)
			{
				dup2(r->heredoc_fd, STDIN_FILENO);
				close(r->heredoc_fd);
				r->heredoc_fd = -1;
			}
		}
		else
			apply_redirections(r, mini);
		r = r->next;
	}
}

static void setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	ft_handle_child(t_cmd *cmd, int prev_fd, int *pipe_fd, 
		int is_last, t_minishell *env)
{
	setup_child_signals();
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
		if (cmd->argv && ft_strcmp(cmd->argv[0], "./minishell") == 0)
		{
			// if (i == 0)
			// 	ft_putstr_fd("can't process minishell inside pipe\n", 1);
			// i++;
			cmd = cmd->next;
			continue ;
		}
		process_intermediate_cmds(cmd, env, &prev_fd);
		cmd = cmd->next;
	}
	// if (cmd->argv && cmd->argv[0])
	// {
	// 	if (ft_strcmp(cmd->argv[0], "./minishell") == 0)
	// 		return ;
	// }
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

static void restore_parent_signals(void (*old_sigint)(int), void (*old_sigquit)(int))
{
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
}

static void	handle_exit_status(int status, t_minishell *env)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			env->exit_status = 130;
			printf("\n");
		}
		else if (sig == SIGQUIT)
		{
			env->exit_status = 131;
			printf("Quit (core dumped)\n");
		}
		else
			env->exit_status = 128 + sig;
	}
	else if (WIFEXITED(status))
		env->exit_status = WEXITSTATUS(status);
}

static void	wait_for_child(pid_t last_pid, t_minishell *env)
{
	pid_t	pid;
	int		status;

	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			handle_exit_status(status, env);
	}
}

void	ft_execute(t_cmd *cmd_list, t_minishell *env)
{
	int		prev_fd;
	pid_t	last_pid;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	prev_fd = -1;
	last_pid = -1;
	if (process_heredocs(cmd_list, env) < 0)
	{
		close_heredocs(cmd_list);
		if (env->exit_status == 130)
			return ;
		env->exit_status = 1;
		return ;
	}
	old_sigint = signal(SIGINT, SIG_IGN);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	if (!cmd_list->next)
		last_pid = execute_single_cmd(cmd_list, env);
	else
		ft_execute_commands(cmd_list, env, prev_fd, &last_pid);
	close_heredocs(cmd_list);
	if (last_pid != -1)
		wait_for_child(last_pid, env);
	restore_parent_signals(old_sigint, old_sigquit);
}
