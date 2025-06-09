/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:16:24 by yhajbi            #+#    #+#             */
/*   Updated: 2025/06/08 19:52:06 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_minishell	*init_minishell(char **env, t_status *e_status);
static t_status		minishell(t_minishell **s_minishell);
static t_cmd		*create_cmd();
static void			print_env(t_env *env);
static void			print_tokens(t_token *s_tokens);
static char			*print_value(int v);
static void			print_cmds(t_cmd *s_cmd);
void 				print_cmd_structure(t_cmd *cmd_list);

/*			---------		MAIN		--------			*/

volatile sig_atomic_t signal_context = 0;

int main(int arc, char **argv, char **env)
{
	t_minishell	*s_minishell;
	t_status	e_status;

	(void)arc;
	(void)argv;
	if (!isatty(STDIN_FILENO))
    	exit (1);
	s_minishell = init_minishell(env, &e_status);
	if (!s_minishell)
		return (e_status);
	while (true)
	{
		e_status = minishell(&s_minishell);
		if (e_status == STATUS_EXIT_CMD)
			break ;
	}
	close(s_minishell->stdfd[0]);
	close(s_minishell->stdfd[1]);
	return (free_minishell(s_minishell), STATUS_SUCCESS);
}

static t_minishell	*init_minishell(char **env, t_status *e_status)
{
	t_minishell	*s_minishell;

	s_minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!s_minishell)
		return (*e_status = STATUS_MALLOC_FAIL, NULL);
	s_minishell->s_env = get_env(env);
	if (!s_minishell->s_env)
		return (*e_status = STATUS_MALLOC_FAIL, NULL);
	s_minishell->cwd = get_env_value(s_minishell->s_env, "PWD");
	if (!s_minishell->cwd)
		return (*e_status = STATUS_MALLOC_FAIL, free_env(s_minishell->s_env), NULL);
	s_minishell->stdfd[0] = dup(STDIN_FILENO);
	s_minishell->stdfd[1] = dup(STDOUT_FILENO);
	return (s_minishell);
}

void	handle_int(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void) sig;
}

void	check_signal(void)
{
	signal(SIGINT, handle_int);
	signal(SIGQUIT, SIG_IGN);
}

static t_status	minishell(t_minishell **s_minishell)
{
	t_minishell	*s_ms;

	s_ms->s_cmd = create_cmd();
	s_ms = *s_minishell;
	check_signal();
	s_ms->cmdline = readline(PROMPT);
	if (!s_ms->cmdline)
	{
		printf("exit\n");
		exit(0);
	}
	if (ft_strcmp(s_ms->cmdline, "exit") == 0)
	{
		rl_clear_history();
		free(s_ms->cmdline);
		return (STATUS_EXIT_CMD);
	}
	add_history(s_ms->cmdline);
	s_ms->s_tokens = ft_tokenizer(s_ms->cmdline);
	if (parse_command_line(s_ms) != STATUS_FAILURE)
		ft_execute(s_ms->s_cmd, s_ms);
	//s_ms->s_cmd = parse(s_ms->s_tokens);
	// print_cmds(s_ms->s_cmd);
	//print_tokens(s_ms->s_tokens);
	// print_cmd_structure(s_ms->s_cmd);
	free(s_ms->cmdline);
	ft_free_tokens(s_ms->s_tokens);
	free_commands(s_ms);
	return (STATUS_SUCCESS);
}

static t_cmd	*create_cmd(void)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = NULL;
	new->is_builtin = 0;
	new->s_redirect = NULL;
	new->next = NULL;
	return (new);
}

/*			--------		TESTING FUNCTIONS		--------			*/

static void	print_env(t_env *env)
{
	t_env	*node;

	node = env;
	while (node)
	{
		printf("%s=%s\n", node->name, node->value);
		node = node->next;
	}
}

static void	print_tokens(t_token *s_tokens)
{
	t_token	*result;

	result = s_tokens;
	printf("/*			******			*/\n");
	while(result)
	{
		printf("value = [%s]\n  type = %s\n", result->value, print_value(result->type));
		printf("---------------------------\n");
		result = result->next;
	}
	printf("/*			******			*/\n");
}

static char	*print_value(int v)
{
	if (v == 0)
		return ("word");
	if (v == 1)
		return ("command");
	if (v == 2)
		return ("string");
	if (v == 3)
		return ("pipe");
	if (v == 4)
		return ("red_i");
	if (v == 5)
		return ("red_o");
	if (v == 6)
		return ("hdoc");
	if (v == 7)
		return ("EOF");
	if (v == 8)
		return ("append");
	if (v == 9)
		return ("file");
	if (v == 10)
		return ("arg");
	if (v == 11)
		return ("var");
	return ("NULL");
}

static void	print_cmds(t_cmd *head)
{
	while (head)
	{
		//printf("Command type: %s\n", print_value(head->type));
		for (int i = 0; head->argv && head->argv[i]; i++)
			printf("  Arg[%d]: %s\n", i, head->argv[i]);
		printf("------------------------------\n");
		head = head->next;
	}
}

// Function to convert token type to string for display
const char *token_type_to_str(t_tokens_type type)
{
    switch (type)
    {
        case TOKEN_WORD: return "TOKEN_WORD";
        case TOKEN_CMD: return "TOKEN_CMD";
        case TOKEN_STR: return "TOKEN_STR";
        case TOKEN_PIPE: return "TOKEN_PIPE";
        case TOKEN_RED_IN: return "TOKEN_RED_IN";
        case TOKEN_RED_OUT: return "TOKEN_RED_OUT";
        case TOKEN_HDOC: return "TOKEN_HDOC";
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_APPEND: return "TOKEN_APPEND";
        case TOKEN_FILE: return "TOKEN_FILE";
        case TOKEN_ARG: return "TOKEN_ARG";
        case TOKEN_VAR: return "TOKEN_VAR";
        default: return "UNKNOWN";
    }
}

// Function to print redirection list
void print_redirects(t_redirect *redirect, int indent)
{
    int i;
    t_redirect *current = redirect;
    
    while (current)
    {
        // Print indentation
        for (i = 0; i < indent; i++)
            printf("  ");
        
        printf("└─ Redirect: type=%s, file='%s'\n", 
               token_type_to_str(current->type), 
               current->file ? current->file : "(null)");
        
        current = current->next;
    }
}

// Function to print argv array
void print_argv(char **argv, int indent)
{
    int i;
    
    if (!argv)
    {
        for (i = 0; i < indent; i++)
            printf("  ");
        printf("└─ argv: (null)\n");
        return;
    }
    
    for (i = 0; i < indent; i++)
        printf("  ");
    printf("└─ argv: [");
    
    for (i = 0; argv[i]; i++)
    {
        printf("\"%s\"", argv[i]);
        if (argv[i + 1])
            printf(", ");
    }
    printf("]\n");
}

// Main function to print command structure
void print_cmd_structure(t_cmd *cmd_list)
{
    int cmd_count = 0;
    t_cmd *current = cmd_list;
    
    printf("Command Structure:\n");
    
    if (!current)
    {
        printf("  (empty command list)\n");
        return;
    }
    
    while (current)
    {
        printf("Command %d:\n", ++cmd_count);
        printf("  ├─ is_builtin: %d\n", current->is_builtin);
        
        // Print argv array
        print_argv(current->argv, 2);
        
        // Print redirections
        if (current->s_redirect)
        {
            printf("  ├─ Redirections:\n");
            print_redirects(current->s_redirect, 3);
        }
        else
        {
            printf("  └─ Redirections: (none)\n");
        }
        
        current = current->next;
        if (current)
            printf("\n");
    }
}