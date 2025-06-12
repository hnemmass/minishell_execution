/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:14:33 by yhajbi            #+#    #+#             */
/*   Updated: 2025/06/11 17:07:37 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//static char		*remove_quotes(char *s);
//static int		count_quotes(char *s);
//static int			has_quotes(char *s);
static void				handle_export_expanding(t_token **head, t_token *node, t_token *prv, t_env *s_env, int exit_status);
static t_export_parse	*expand_export_fragments(char **split, t_env *s_env, int exit_status);
static void				addlst_export_parse(t_export_parse **head, char	*new_frag);
static void				split_after_expand(t_export_parse **head);
static void				find_and_join_equal(t_export_parse **head);
static void				join_three_nodes(t_export_parse *prv, t_export_parse *node, t_export_parse *next);
static void				split_and_insert_expanded(t_export_parse **head, t_export_parse *node);
static void				insert_expanded_in_tokens(t_token **start, t_export_parse *head);
static char				*remove_quotes(char *s, t_env *s_env, int exit_status);
static int				is_word(t_tokens_type type);
static t_substring		*split_quotes(char *s);
static void				add_substring(t_substring **head, char *s, char quote);
static t_quotes			identify_quotes(char quote);
static int				count_substrings(char *s);
static char				*join_substrings(t_substring *head);
static void				split_and_insert_tokens(t_token **head, t_token *node);
static void 			change_tabs(char *line);
static void				free_split(char **split);

void	handle_quotes(t_token *s_tokens, t_env *s_env, int exit_status)
{
	t_token	*node;
	t_token	*prv;
	char	*old_value;
	int		export_flag;

	node = s_tokens;
	prv = s_tokens;
	old_value = NULL;
	export_flag = 0;
	while (node)
	{
		old_value = node->value;
		if (ft_strcmp(node->value, "export") == 0)
			export_flag = 1;
		else if (node->type == TOKEN_PIPE)
			export_flag = 0;
		if (has_quotes(node->value))
		{
			node->value = remove_quotes(node->value, s_env, exit_status);
			if (old_value != NULL)
				free(old_value);
		}
		if (export_flag == 0)
			split_and_insert_tokens(&s_tokens, node);
		else
		{
			handle_export_expanding(&s_tokens, node, prv, s_env, exit_status);
		}
		if (!has_quotes(node->value) && is_word(node->type))
		{
			node->value = scan_string(node->value, s_env, exit_status);
			//if (old_value != NULL)
			//	free(old_value);
			/*if ((ft_strchr(node->value, ' ') || ft_strchr(node->value, '\t'))
				&& ft_strcmp("export", prv->value)  == 0 && !ft_strchr(node->value, '='))
				split_and_insert_tokens(&s_tokens, node, prv);
			else if ((ft_strchr(node->value, ' ') || ft_strchr(node->value, '\t'))
				&& ft_strcmp("export", prv->value) != 0)
				split_and_insert_tokens(&s_tokens, node, prv);*/
		}
		prv = node;
		node = node->next;
	}
}

static void	handle_export_expanding(t_token **head, t_token *node, t_token *prv, t_env *s_env, int exit_status)
{
	t_export_parse	*exp_head;
	char			**split;

	if (ft_strchr(node->value, '$') == NULL)
		return ;
	split = ft_split(node->value, "=");
	exp_head = expand_export_fragments(split, s_env, exit_status);
	split_after_expand(&exp_head);
	find_and_join_equal(&exp_head);
	insert_expanded_in_tokens(&prv, exp_head);
	/*t_token *current = prv;
	while (current) {
		printf("Token: [%s] Type: %d\n", current->value, current->type);
		current = current->next;
	}*/
	/*while (exp_head)
	{
		printf("[%s]		|		was_expanded ==> [%d]\n", exp_head->fragment, exp_head->type);
		exp_head = exp_head->next;
	}*/
	/*t_token *current = *head;
    while (current) {
        printf("Token: [%s] Type: %d\n", current->value, current->type);
        current = current->next;
    }*/
}

static t_export_parse	*expand_export_fragments(char **split, t_env *s_env, int exit_status)
{
	t_export_parse	*head;
	t_export_parse	*node;
	char			*old_value;
	int				i;

	head = NULL;
	i = 0;
	while (split[i])
	{
		addlst_export_parse(&head, split[i++]);
		if (split[i])
			addlst_export_parse(&head, "=");
	}
	node = head;
	while (node)
	{
		old_value = node->fragment;
		node->fragment = scan_string(node->fragment, s_env, exit_status);
		if (ft_strcmp(old_value, node->fragment) != 0)
			node->type = WAS_EXPANDED;
		if (old_value != NULL)
			free(old_value);
		node = node->next;
	}
	return (head);
}

static void	addlst_export_parse(t_export_parse **head, char	*new_frag)
{
	t_export_parse	*node;
	t_export_parse	*new;

	node = *head;
	if (!node)
	{
		*head = malloc(sizeof(t_export_parse));
		if (!(*head))
			return ;
		(*head)->fragment = ft_strdup(new_frag);
		(*head)->type = WAS_NOT_EXPANDED;
		(*head)->next = NULL;
		return ;
	}
	while (node->next)
		node = node->next;
	new = malloc(sizeof(t_export_parse));
	if (!new)
		return ;
	new->fragment = ft_strdup(new_frag);
	new->type = WAS_NOT_EXPANDED;
	new->next = NULL;
	node->next = new;
}

static void	split_after_expand(t_export_parse **head)
{
	int				i;
	t_export_parse	*node;

	i = 1;
	node = *head;
	while (node)
	{
		if (node->type == WAS_EXPANDED && i == 1)
			split_and_insert_expanded(head, node);
		node = node->next;
	}
}

static void	find_and_join_equal(t_export_parse **head)
{
	t_export_parse	*node;
	t_export_parse	*next;
	t_export_parse	*prv;

	node = *head;
	next = node->next->next;
	prv = node;
	while (node)
	{
		if (!(node->next))
			break ;
		next = node->next->next;
		if (ft_strcmp("=", node->fragment) == 0)
			join_three_nodes(prv, node, next);
		node = next;
	}
}

static void	join_three_nodes(t_export_parse *prv, t_export_parse *node, t_export_parse *next)
{
	char	*new_frag;
	
	new_frag = malloc(sizeof(char) * (ft_strlen(prv->fragment)
		+ ft_strlen(node->fragment) + ft_strlen(next->fragment) + 1));
	ft_strlcpy(new_frag, prv->fragment, ft_strlen(prv->fragment));
	ft_strlcpy(new_frag, node->fragment, ft_strlen(node->fragment));
	ft_strlcpy(new_frag, next->fragment, ft_strlen(next->fragment));
	new_frag[ft_strlen(prv->fragment) + ft_strlen(node->fragment) + ft_strlen(next->fragment)] = '\0';
	printf("joined string = [%s]\n", new_frag);
}

static void	split_and_insert_expanded(t_export_parse **head, t_export_parse *node)
{
	char			**split_words;
	t_export_parse	*new_token;
	t_export_parse	*next_node;
	t_export_parse	*rest;
	int				i;

	split_words = ft_split(node->fragment, " ");
	if (!split_words)
		return ;
	free(node->fragment);
	node->fragment = ft_strdup(split_words[0]);
	node->type = WAS_NOT_EXPANDED;
	rest = node->next;
	i = 1;
	while (split_words[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			break ;
		new_token->fragment = ft_strdup(split_words[i]);
		new_token->type = WAS_EXPANDED;
		new_token->next = next_node;
		node->next = new_token;
		node = new_token;
		i++;
	}
	node->next = rest;
	free_split(split_words);
}

static void	insert_expanded_in_tokens(t_token **start, t_export_parse *head)
{
	t_token			*node;
	t_export_parse	*exp_node;
	t_token			*new_token;
	t_token			*next_node;
	t_token			*rest;

	node = *start;
	exp_node = head;
	rest = node->next;
	while (exp_node)
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return ;
		new_token->value = ft_strdup(exp_node->fragment);
		new_token->type = TOKEN_ARG;
		new_token->next = NULL;
		node->next = new_token;
		node = node->next;
		exp_node = exp_node->next;
	}
	node->next = rest;
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

static void	split_and_insert_tokens(t_token **head, t_token *node)
{
	char	**split_words;
	t_token	*new_token;
	t_token	*next_node;
	t_token	*rest;
	int		i;

	change_tabs(node->value);
	split_words = ft_split(node->value, " ");
	if (!split_words)
		return ;
	free(node->value);
	node->value = ft_strdup(split_words[0]);
	rest = node->next;
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
	node->next = rest;
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
