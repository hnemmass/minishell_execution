/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:03:11 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/07 17:57:22 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct	s_env			t_env;
typedef struct	s_token			t_token;
typedef struct	s_cmd			t_cmd;
typedef struct	s_redirect		t_redirect;
typedef struct	s_parse			t_parse;
typedef struct	s_substring		t_substring;

typedef struct	s_minishell
{
	char	*cmdline;
	t_env	*s_env;
	char	*cwd;
	t_token	*s_tokens;
	t_cmd	*s_cmd;
	int		stdfd[2];
	int		exit_status;
}			t_minishell;

typedef struct	s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef enum	e_status
{
	STATUS_SUCCESS,
	STATUS_FAILURE,
	STATUS_MALLOC_FAIL,
	STATUS_SYNTAX_ERR,
	STATUS_EXIT_CMD
}				t_status;

typedef enum	e_tokens_type
{
	TOKEN_WORD,		/*CMD OR ARG*/ 
	TOKEN_CMD,
	TOKEN_STR,
	TOKEN_PIPE,		/* | */
	TOKEN_RED_IN,	/* < */
	TOKEN_RED_OUT,	/* > */
	TOKEN_HDOC,		/* << */
	TOKEN_EOF,
	TOKEN_APPEND,	/* >> */
	TOKEN_FILE,
	TOKEN_ARG,
	TOKEN_VAR
}		t_tokens_type;

typedef struct	s_token
{
	char			*value;
	t_tokens_type	type;
	struct s_token	*next;
}		t_token;

typedef struct	s_cmd
{
	char			**argv;
	//t_tokens_type	type;
	int				is_builtin;
	t_redirect		*s_redirect;
	struct s_cmd	*next;
}					t_cmd;

typedef struct	s_redirect
{
	t_tokens_type		type;
	char				*file;
	struct s_redirect	*next;
}						t_redirect;

/*	----	FOR PARSING		----	*/

typedef enum	e_quotes
{
	UNQUOTED,
	DOUBLE_QUOTED,
	SINGLE_QUOTED
}				t_quotes;

typedef struct	s_substring
{
	char				*str;
	t_quotes			type;
	struct s_substring	*next;
}				t_substring;

#endif