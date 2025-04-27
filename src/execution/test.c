/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:45:12 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/27 15:31:40 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* test_execution.c */
#include "../../inc/execution.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

t_env	*create_env_node(char *name, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **env, t_env *new_node)
{
	t_env	*current;

	if (!*env)
	{
		*env = new_node;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	char	**split;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], '=');
		if (split[0] && split[1])
			add_env_node(&env, create_env_node(split[0], split[1]));
		free_split(split);
		i++;
	}
	return (env);
}

t_redirect	*create_redirect(t_tokens_type type, char *file)
{
	t_redirect	*red;

	red = malloc(sizeof(t_redirect));
	if (!red)
		return (NULL);
	red->type = type;
	red->file = ft_strdup(file);
	red->next = NULL;
	return (red);
}

void	add_redirect(t_redirect **red, t_redirect *new_red)
{
	t_redirect	*current;

	if (!*red)
	{
		*red = new_red;
		return ;
	}
	current = *red;
	while (current->next)
		current = current->next;
	current->next = new_red;
}

t_cmd	*create_cmd(char **argv, int is_builtin)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = argv;
	cmd->is_builtin = is_builtin;
	cmd->s_redirect = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd(t_cmd **cmd, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!*cmd)
	{
		*cmd = new_cmd;
		return ;
	}
	current = *cmd;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

// Free functions
void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	free_redirect(t_redirect *red)
{
	t_redirect	*tmp;

	while (red)
	{
		tmp = red;
		red = red->next;
		free(tmp->file);
		free(tmp);
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		i = 0;
		while (tmp->argv[i])
			free(tmp->argv[i++]);
		free(tmp->argv);
		free_redirect(tmp->s_redirect);
		free(tmp);
	}
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] - s2[i] != 0)
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

static void	free_allocated_strings(char **s1, int count)
{
	while (count > 0)
	{
		free(s1[count - 1]);
		count--;
	}
	free(s1);
}

static int	count_w(char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i])
		{
			j++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (j);
}

static char	**allocate_a_cpy(char const *s, char **s1, int start, int end)
{
	*s1 = ft_substr(s, start, (end - start));
	if (!*s1)
		return (NULL);
	return (s1);
}

static char	**count_a_cpy(char const *s, char **s1, char c)
{
	int	i;
	int	start;
	int	index;

	index = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] != c && s[i])
			i++;
		if (start < i)
		{
			if (allocate_a_cpy(s, s1 + index, start, i) == NULL)
			{
				free_allocated_strings(s1, index);
				return (NULL);
			}
			index++;
		}
	}
	s1[index] = NULL;
	return (s1);
}

char	**ft_split(char const *s, char c)
{
	size_t	total_count;
	char	**s1;

	if (!s)
		return (NULL);
	total_count = count_w(s, c) + 1;
	s1 = malloc(total_count * sizeof(char *));
	if (!s1)
		return (NULL);
	if (count_a_cpy(s, s1, c) == NULL)
		return (NULL);
	return (s1);
}


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
	if (!env_array)
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

static void
ft_handle_child(t_cmd *cmd, int prev_fd, int *pipe_fd, int is_last, t_minishell *env)
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

static void
ft_execute_commands(t_cmd *cmd, t_minishell *env, int prev_fd, pid_t *last_pid)
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


// void	test_simple_cmd(t_minishell *shell)
// {
// 	char	**argv;

// 	printf("\n----- Testing Simple Command: ls -la -----\n");
// 	argv = malloc(sizeof(char *) * 3);
// 	argv[0] = ft_strdup("ls");
// 	argv[1] = ft_strdup("-la");
// 	argv[2] = NULL;
	
// 	shell->s_cmd = create_cmd(argv, 0);
// 	ft_execute(shell->s_cmd, shell);
// 	free_cmd(shell->s_cmd);
// 	shell->s_cmd = NULL;
// }

// void	test_stdin_cat(t_minishell *shell)
// {
// 	char	**argv;
// 	t_cmd	*cmd;

// 	printf("\n----- Testing cat from stdin (press Ctrl+D to exit) -----\n");
// 	argv = malloc(sizeof(char *) * 2);
// 	argv[0] = ft_strdup("cat");
// 	argv[1] = NULL;
	
// 	shell->s_cmd = create_cmd(argv, 0);
// 	ft_execute(shell->s_cmd, shell);
// 	free_cmd(shell->s_cmd);
// 	shell->s_cmd = NULL;
// }

// void	test_output_redirection(t_minishell *shell)
// {
// 	char	**argv;
// 	t_cmd	*cmd;

// 	printf("\n----- Testing Output Redirection: ls -la > output.txt -----\n");
// 	argv = malloc(sizeof(char *) * 3);
// 	argv[0] = ft_strdup("ls");
// 	argv[1] = ft_strdup("-la");
// 	argv[2] = NULL;
	
// 	shell->s_cmd = create_cmd(argv, 0);
// 	add_redirect(&shell->s_cmd->s_redirect, create_redirect(TOKEN_RED_OUT, "output.txt"));
// 	ft_execute(shell->s_cmd, shell);
// 	free_cmd(shell->s_cmd);
// 	shell->s_cmd = NULL;
// }

// void test_pipe_and_redirections(t_minishell *shell)
// {
//     char    **argv1, **argv2;
//     t_cmd   *cmd1, *cmd2;

//     printf(
//         "\n----- Testing Pipe + Multiple Redirections: "
//         "cat < missing.txt < output.txt | grep foo > out1.txt > out2.txt -----\n"
//     );
//     argv1 = malloc(sizeof(char *) * 2);
//     argv1[0] = ft_strdup("cat");
//     argv1[1] = NULL;
//     cmd1 = create_cmd(argv1, 0);
//     add_redirect(&cmd1->s_redirect, create_redirect(TOKEN_RED_IN, "missing.txt"));
//     add_redirect(&cmd1->s_redirect, create_redirect(TOKEN_RED_IN, "redirections.c"));

//     argv2 = malloc(sizeof(char *) * 3);
//     argv2[0] = ft_strdup("grep");
//     argv2[1] = ft_strdup("while");
//     argv2[2] = NULL;
//     cmd2 = create_cmd(argv2, 0);
//     add_redirect(&cmd2->s_redirect, create_redirect(TOKEN_RED_OUT, "out1.txt"));
//     add_redirect(&cmd2->s_redirect, create_redirect(TOKEN_RED_OUT, "out2.txt"));

//     cmd1->next = cmd2;
//     shell->s_cmd = cmd1;
//     ft_execute(shell->s_cmd, shell);
//     free_cmd(shell->s_cmd);
//     shell->s_cmd = NULL;
// }



// void	test_append_redirection(t_minishell *shell)
// {
// 	char	**argv;
// 	t_cmd	*cmd;

// 	printf("\n----- Testing Append Redirection: echo \"append test\" >> output.txt -----\n");
// 	argv = malloc(sizeof(char *) * 3);
// 	argv[0] = ft_strdup("echo");
// 	argv[1] = ft_strdup("append test");
// 	argv[2] = NULL;
	
// 	shell->s_cmd = create_cmd(argv, 1);
// 	add_redirect(&shell->s_cmd->s_redirect, create_redirect(TOKEN_APPEND, "output.txt"));
// 	ft_execute(shell->s_cmd, shell);
// 	free_cmd(shell->s_cmd);
// 	shell->s_cmd = NULL;
// }

// void	test_pipe(t_minishell *shell)
// {
// 	char	**argv1;
// 	char	**argv2;
// 	t_cmd	*cmd1;
// 	t_cmd	*cmd2;

// 	printf("\n----- Testing Pipe: cat  | ls -----\n");
// 	argv1 = malloc(sizeof(char *) * 2);
// 	argv1[0] = ft_strdup("cat");
// 	// argv1[1] = ft_strdup("-la");
// 	argv1[1] = NULL;
	
// 	argv2 = malloc(sizeof(char *) * 2);
// 	argv2[0] = ft_strdup("ls");
// 	// argv2[1] = ft_strdup(".c");
// 	argv2[1] = NULL;
	
// 	cmd1 = create_cmd(argv1, 0);
// 	cmd2 = create_cmd(argv2, 0);
// 	cmd1->next = cmd2;
	
// 	shell->s_cmd = cmd1;
// 	ft_execute(shell->s_cmd, shell);
// 	free_cmd(shell->s_cmd);
// 	shell->s_cmd = NULL;
// }

// void test_pipe_with_empty_command_redirection(t_minishell *shell)
// {
//     char **argv1;
//     char **argv2;
//     t_cmd *cmd1;
//     t_cmd *cmd2;

//     printf("\n----- Testing Pipe with Empty Command Redirection: ls | > out -----\n");
    
//     // First command: ls
//     argv1 = malloc(sizeof(char *) * 2);
//     argv1[0] = ft_strdup("ls");
//     argv1[1] = NULL;
    
//     // Second command: empty command with redirection
//     argv2 = malloc(sizeof(char *) * 1);
//     argv2[0] = NULL;
    
//     cmd1 = create_cmd(argv1, 0);
//     cmd2 = create_cmd(argv2, 0);
//     add_redirect(&cmd2->s_redirect, create_redirect(TOKEN_RED_OUT, "out"));
//     cmd1->next = cmd2;
    
//     shell->s_cmd = cmd1;
//     ft_execute(shell->s_cmd, shell);
    
//     // Check if the redirection worked
//     printf("Contents of 'out' file:\n");
//     system("cat out");
    
//     // Clean up
//     free_cmd(shell->s_cmd);
//     shell->s_cmd = NULL;
    
//     // Restore standard file descriptors for next test
//     dup2(shell->stdfd[0], STDIN_FILENO);
//     dup2(shell->stdfd[1], STDOUT_FILENO);
// }

void	test_pipe_with_redirection(t_minishell *shell)
{
	char	**argv1;
	char	**argv2;
	t_cmd	*cmd1;
	t_cmd	*cmd2;

	printf("\n----- Testing Pipe with Redirection: sleep 3 | export a=5 > grep_output.txt -----\n");
	argv1 = malloc(sizeof(char *) * 3);
	argv1[0] = ft_strdup("sleep");
	argv1[1] = ft_strdup("3");
	argv1[2] = NULL;
	
	argv2 = malloc(sizeof(char *) * 3);
	argv2[0] = ft_strdup("export");
	argv2[1] = ft_strdup("a=5");
	argv2[2] = NULL;
	
	cmd1 = create_cmd(argv1, 0);
	cmd2 = create_cmd(argv2, 0);
	// add_redirect(&cmd2->s_redirect, create_redirect(TOKEN_RED_OUT, "grep_output.txt"));
	// cmd1->next = cmd2;
	
	shell->s_cmd = cmd1;
	ft_execute(shell->s_cmd, shell);
	free_cmd(shell->s_cmd);
	shell->s_cmd = NULL;
}

// void	test_builtin_command(t_minishell *shell)
// {
// 	char	**argv;

// 	printf("\n----- Testing Built-in Command: export a+=6 world-----\n");
// 	argv = malloc(sizeof(char *) * 4);
// 	argv[0] = ft_strdup("export");
// 	argv[1] = ft_strdup("a+=6");
// 	argv[2] = ft_strdup("World");
// 	argv[3] = NULL;
	
// 	shell->s_cmd = create_cmd(argv, 1);
// 	ft_execute(shell->s_cmd, shell);
// 	free_cmd(shell->s_cmd);
// 	shell->s_cmd = NULL;
// }

void	test_builtin_pwd(t_minishell *shell)
{
	char	**argv;

	printf("\n----- Testing Built-in Command: export -----\n");
	argv = malloc(sizeof(char *) * 2);
	argv[0] = ft_strdup("export");
	argv[1] = NULL;
	
	shell->s_cmd = create_cmd(argv, 1);
	ft_execute(shell->s_cmd, shell);
	free_cmd(shell->s_cmd);
	shell->s_cmd = NULL;
}

// void	test_builtin_cd(t_minishell *shell)
// {
// 	char	**argv;

// 	printf("\n----- Testing Built-in Command: env -----\n");
// 	argv = malloc(sizeof(char *) * 3);
// 	argv[0] = ft_strdup("env");
// 	// argv[1] = ft_strdup("..");
// 	argv[1] = NULL;
	
// 	shell->s_cmd = create_cmd(argv, 1);
// 	ft_execute(shell->s_cmd, shell);
// 	free_cmd(shell->s_cmd);
// 	shell->s_cmd = NULL;
	
// 	// Verify new directory
// 	argv = malloc(sizeof(char *) * 2);
// 	argv[0] = ft_strdup("pwd");
// 	argv[1] = NULL;
	
// 	shell->s_cmd = create_cmd(argv, 1);
// 	ft_execute(shell->s_cmd, shell);
// 	free_cmd(shell->s_cmd);
// 	shell->s_cmd = NULL;
// }

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;

	(void)argc;
	(void)argv;
	
	// Initialize shell
	shell.s_env = init_env(envp);
	shell.s_cmd = NULL;
	shell.cmdline = NULL;
	shell.s_tokens = NULL;
	
	// Save standard input/output
	shell.stdfd[0] = dup(STDIN_FILENO);
	shell.stdfd[1] = dup(STDOUT_FILENO);
	
	// Run tests
    // test_simple_cmd(&shell);
    // test_stdin_cat(&shell);
    // test_output_redirection(&shell);
    // test_pipe_and_redirections(&shell);
    // test_append_redirection(&shell);
    // test_pipe(&shell);
    test_pipe_with_redirection(&shell);
    // test_pipe_with_empty_command_redirection(&shell); // Add this line
    // test_builtin_command(&shell);
    // test_builtin_pwd(&shell);
    // test_builtin_cd(&shell);
	
	// Free resources
	free_env(shell.s_env);
	
	return (0);
}

