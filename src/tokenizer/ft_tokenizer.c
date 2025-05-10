/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:29:49 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/24 12:09:17 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*char	*ft_strdup(char *s)
{
	char	*d_s;
	char	*d;

	d_s = (char *)malloc(strlen(s) + 1);
	if (!d_s)
		return (NULL);
	d = d_s;
	while (*s)
	{
		*d_s = *s;
		d_s++;
		s++;
	}
	*d_s = '\0';
	return (d);
}*/

int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

t_token	*ft_create_token(char *line, t_tokens_type type, int i)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(line);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	if (i == 1)
		free(line);
	return (new_token);
}

t_token	*handle_mchar(char *line, int *i)
{
	char	op[3];
	int		op_len;

	op_len = 1;
	op[0] = line[*i];
	op[1] = '\0';
	op[2] = '\0';
	if ((line[*i] == '<' || line[*i] == '>') && line[*i + 1] == line[*i])
	{
		op[1] = line[*i];
		op_len = 2;
		(*i) += 2;
	}
	else
		(*i)++;
	if (op[0] == '|')
		return (ft_create_token(op, TOKEN_PIPE, 0));
	else if (op[0] == '<' && op_len == 1)
		return (ft_create_token(op, TOKEN_RED_IN, 0));
	else if (op[0] == '>' && op_len == 1)
		return (ft_create_token(op, TOKEN_RED_OUT, 0));
	else if (op[0] == '<' && op_len == 2)
		return (ft_create_token(op, TOKEN_HDOC, 0));
	else if (op[0] == '>' && op_len == 2)
		return (ft_create_token(op, TOKEN_APPEND, 0));
	return (NULL);
}

void	check_space(char *line, int *i)
{
	while (ft_isspace(line[*i]))
		(*i)++;
	return ;
}

t_token *handle_word(char *line, int *i)
{
    char *word;
    int j;
    int start;
    char quote;

    check_space(line, i);
    start = *i;
    quote = 0;
    while(line[*i])
    {
        if (!quote && (line[*i] == '<' || line[*i] == '>' || line[*i] == '|' || ft_isspace(line[*i])))
            break; 
        if ((line[*i] == '\'' || line[*i] == '\"'))
        {
            if (!quote)
            {
                quote = line[*i];
                (*i)++;
                while (line[*i] && line[*i] != quote)
                    (*i)++;
                if (!line[*i])
                    return (NULL);
                (*i)++;
                quote = 0;
            }
            else if (line[*i] == quote)
            {
                (*i)++;
                quote = 0;
            }
            else
                (*i)++;
        }
        else
            (*i)++;
    }
    word = malloc((*i) - start + 1);
    if (!word)
        return (NULL);
    j = 0;
    while (start < (*i))
        word[j++] = line[start++];
    word[j] = '\0';
    return (ft_create_token(word, TOKEN_WORD, 1));
}

void	ft_free_tokens(t_token *head)
{
	t_token	*temp;

	if (head == NULL)
		return ;
	while(head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

void	ft_add_token(t_token **head, t_token **current, t_token *new_token)
{
	if (!(*head))
	{
		(*head) = new_token;
		(*current) = new_token;
	}
	else 
	{
		(*current)->next = new_token;
		(*current) = new_token;
	}
}

t_token	*ft_tokenizer(char *line)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
		{
			i++;
			continue ;
		}
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			new_token = handle_mchar(line, &i);
		else
			new_token = handle_word(line, &i);
		if (!new_token)
		{
			ft_free_tokens(head);
			return (NULL);
		}
		ft_add_token(&head, &current, new_token);
	}
	return (head);
}

/*int main(int ac, char **v)
{
	if (!v[1] || ac == 1)
		return (1);
	char *c = v[1];
	t_token *result;
	printf("------------%s\n", v[1]);
	result = ft_tokenizer(v[1]);
	t_token *head = result;
	if (!result)
	{
		printf("error");
		return (1);
	}
	while(result)
	{
		printf("value = [%s]\n  type = %d\n", result->value, result->type);
		result = result->next;
	}
	ft_free_tokens(head);
}*/