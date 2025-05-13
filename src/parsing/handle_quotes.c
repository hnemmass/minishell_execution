/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:14:33 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/09 18:21:45 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//static char		*remove_quotes(char *s);
//static int		count_quotes(char *s);
//static int			has_quotes(char *s);
static char			*remove_quotes(char *s, t_env *s_env, int exit_status);
static int			is_word(t_tokens_type type);
static t_substring	*split_quotes(char *s);
static void			add_substring(t_substring **head, char *s, char quote);
static t_quotes		identify_quotes(char quote);
static int			count_substrings(char *s);
static char			*join_substrings(t_substring *head);
static void			split_and_insert_tokens(t_token **head, t_token *node, t_token *prv);
static void 		change_tabs(char *line);
static void			free_split(char **split);

void	handle_quotes(t_token *s_tokens, t_env *s_env, int exit_status)
{
	t_token	*node;
	t_token	*prv;
	char	*old_value;

	node = s_tokens;
	prv = s_tokens;
	old_value = NULL;
	while (node)
	{
		old_value = node->value;
		if (has_quotes(node->value))
		{
			node->value = remove_quotes(node->value, s_env, exit_status);
			if (old_value != NULL)
				free(old_value);
		}
		else if (!has_quotes(node->value) && is_word(node->type))
		{
			node->value = scan_string(node->value, s_env, exit_status);
			if (old_value != NULL)
				free(old_value);
			if ((ft_strchr(node->value, ' ') || ft_strchr(node->value, '\t'))
				&& ft_strcmp("export", prv->value)  == 0 && !ft_strchr(node->value, '='))
				split_and_insert_tokens(&s_tokens, node, prv);
			else if ((ft_strchr(node->value, ' ') || ft_strchr(node->value, '\t'))
				&& ft_strcmp("export", prv->value) != 0)
				split_and_insert_tokens(&s_tokens, node, prv);
		}
		prv = node;
		node = node->next;
	}
}

int	has_quotes(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

static int	is_word(t_tokens_type type)
{
	return (type == TOKEN_CMD || type == TOKEN_ARG || type == TOKEN_VAR
		|| type == TOKEN_WORD || type == TOKEN_FILE || type == TOKEN_APPEND
		|| type == TOKEN_FILE);
}

static char	*remove_quotes(char *s, t_env *s_env, int exit_status)
{
	t_substring	*list;
	char		*ret;

	list = split_quotes(s);
	if (!list)
		return (NULL);
	t_substring	*node = list;
	/*while (node)
	{
		printf("[%s] --> %d\n", node->str, node->type);
		node = node->next;
	}*/
	expand_variables(&list, s_env, exit_status);
	ret = join_substrings(list);
	//printf("[%s]\n", ret);
	/*free_substrings(list);
	if (!ret)
		return (NULL);*/
	return (ret);
}

static t_substring	*split_quotes(char *s)
{
	t_substring	*list;
	int			i;
	int			start;
	char		cur_quote;

	list = NULL;
	i = 0;
	start = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
		{
			if (start != i) // Save text before quote
				add_substring(&list, ft_substr(s, start, i - start), 0);
			cur_quote = s[i];
			start = i + 1;
		}
		else if (s[i] == cur_quote)
		{
			add_substring(&list, ft_substr(s, start, i - start), cur_quote);
			cur_quote = '\0';
			start = i + 1;
		}
		i++;
	}
	if (start < i) // Save leftover text after the loop
		add_substring(&list, ft_substr(s, start, i - start), 0);
	return (list);
}

/*static t_substring	*split_quotes(char *s)
{
	t_substring	*list;
	int			i;
	int			start;
	int			end;
	char		cur_quote;

	list = NULL;
	i = 0;
	start = 0;
	end = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
		{
			cur_quote = s[i];
			start = i + 1;
		}
		else if (s[i] == cur_quote && s[i])
		{
			end = i;
			add_substring(&list, ft_substr(s, start, end - start), cur_quote);
			cur_quote = '\0';
		}
		i++;
	}
	return (list);
}*/

static void	add_substring(t_substring **head, char *s, char quote)
{
	t_substring	*new;
	t_substring	*node;

	new = malloc(sizeof(t_substring));
	if (!new)
		return ;
	new->str = s;
	new->type = identify_quotes(quote);
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return ;
	}
	node = *head;
	while (node->next)
		node = node->next;
	node->next = new;
}

static t_quotes	identify_quotes(char quote)
{
	if (quote == '\'')
		return (SINGLE_QUOTED);
	else if (quote == '\"')
		return (DOUBLE_QUOTED);
	return (UNQUOTED);
}

static int	count_substrings(char *s)
{
	int		i;
	int		count;
	char	cur_quote;

	i = 0;
	count = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
			cur_quote = s[i];
		else if (s[i] == cur_quote && s[i])
		{
			cur_quote = '\0';
			count++;
		}
		i++;
	}
	return (count);
}

static char	*join_substrings(t_substring *head)
{
	t_substring	*node;
	char		*ret;
	int			len;
	int			i;

	node = head;
	len = 0;
	while(node)
	{
		len += ft_strlen(node->str);
		node = node->next;
	}
	ret = NULL;
	i = 0;
	node = head;
	while (node)
	{
		ret = ft_strconcat(ret, node->str);
		if (!ret)
			return (NULL);
		node = node->next;
	}
	return (ret);
}

static void	split_and_insert_tokens(t_token **head, t_token *node, t_token *prv)
{
	char	**split_words;
	t_token	*new_token;
	t_token	*next_node;
	int		i;

	change_tabs(node->value);
	split_words = ft_split(node->value, " ");
	if (!split_words)
		return ;
	free(node->value);
	node->value = ft_strdup(split_words[0]);
	i = 1;
	while (split_words[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			break ;
		new_token->value = ft_strdup(split_words[i]);
		new_token->type = node->type;
		new_token->next = next_node;
		node->next = new_token;
		node = new_token;
		i++;
	}
	node->next = NULL;
	free_split(split_words);
}

static void change_tabs(char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if (line[i] == '\t')
            line[i] = ' ';
        i++;
    }
}

static void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
}

/*static char	*remove_quotes(char *s)
{
	int		i;
	int		j;
	char	cur_quote;
	char	*ret;

	i = 0;
	j = 0;
	cur_quote = '\0';
	ret = malloc(sizeof(char) * ft_strlen(s) - count_quotes(s) + 1);
	if (!ret)
		return (NULL);
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
			cur_quote = s[i];
		else if (s[i] == cur_quote && s[i])
			cur_quote = '\0';
		else
		{
			ret[j] = s[i];
			j++;
		}
		i++;
	}
	return (ret);
}

static int	count_quotes(char *s)
{
	int	i;
	int	count;
	int	cur_quote;

	i = 0;
	count = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
		{
			cur_quote = s[i];
			count++;
		}
		else if (s[i] == cur_quote)
		{
			cur_quote = '\0';
			count++;
		}
		i++;
	}
	return (count);
}*/
