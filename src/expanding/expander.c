/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:06:02 by yhajbi            #+#    #+#             */
/*   Updated: 2025/05/09 13:20:36 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//static char	*scan_string(char *s, t_env *s_env);
static char	*str_append_str(char *s1, char *s2);
static int	is_valid_var(char *s, t_env *s_env);
static int	ft_strlen_var(char *s);
static int	is_delimiter(char c);
static char	*str_append_char(char *s, char c);

void	expand_variables(t_substring **head, t_env *s_env)
{
	t_substring	*node;
	char		*old_value;

	node = *head;
	while (node)
	{
		old_value = node->str;
		if (has_var(node->str) && (node->type == DOUBLE_QUOTED || node->type == UNQUOTED))
		{
			node->str = scan_string(node->str, s_env);
			if (old_value != NULL)
				free(old_value);
		}
		node = node->next;
	}
}

char	*scan_string(char *s, t_env *s_env)
{
	int		i;
	char	*ret;

	i = 0;
	ret = ft_calloc(1, 1);
	while (s[i])
	{
		if (s[i] == '$')
		{
			if (s[i + 1] == '$')
			{
				ret = str_append_str(ret, "$$");
				i += 2;
			}
			else if (is_delimiter(s[i + 1]))
			{
				ret = str_append_char(ret, '$');
				i++;
			}
			else if (has_var(s + i)/*is_valid_var(s + i + 1, s_env)*/)
			{
				ret = str_append_str(ret, get_env_value2(s_env, s + i));
				i += ft_strlen_var(s + i + 1) + 1;
			}
		}
		else
		{
			ret = str_append_char(ret, s[i]);
			i++;
		}
	}
	return (ret);
}

static char	*str_append_str(char *s1, char *s2)
{
	char	*new_str;
	int		len1;
	int		len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new_str = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, len1 + 1);
	ft_strlcpy(new_str + len1, s2, len2 + 1);
	free(s1);
	return (new_str);
}

static int	is_valid_var(char *s, t_env *s_env)
{
	t_env	*node;

	node = s_env;
	while (node)
	{
		if (ft_strcmp_env(node->name, s) == 0)
			return (1);
		node = node->next;
	}
	return (0);
}

static int	ft_strlen_var(char *s)
{
	int	i;

	i = 0;
	while (s[i] && !is_delimiter(s[i]))
		i++;
	return (i);
}

static int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r'
		|| c == '\'' || c == '$' || c == '\0'
		|| c == '+' || c == '-' || c == '.'
		|| c == '[' || c == ']' || c == '{'
		|| c == '}' || c == '(' || c == ')');
	/*return (c == ' ' || c == '\'' || c == '\"'
		|| c == '$' || c == '\0');*/
}

static char	*str_append_char(char *s, char c)
{
	char	*new_str;
	int		len;

	len = ft_strlen(s);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s, len + 1);
	ft_strlcpy(new_str + len, &c, 2);
	free(s);
	return (new_str);
}

/*static int	count_split(char **split);
static void	expand_words(char **split, t_env *s_env, int flag1, int flag2);
static char	*add_dollar_end(char **s);
static int	is_delimiter(char c);
static char	*add_dollar(char **s);
static int	lookup_var(t_env *s_env, char *name);
static char	*expand(char *s, t_env *s_env);
static char	*handle_unknown_var(char *s);
static char	*copy_env_with_space(const char *env_value, const char *space);
static char	*get_space_start(char **s);
static char *join_words(char **split, int count);
static char	*handle_pid(char **s);
static void	free_split(char **split);

void	expand_variables(t_substring **head, t_env *s_env)
{
	t_substring	*node;
	char		*old_value;

	node = *head;
	while (node)
	{
		old_value = node->str;
		if (has_var(node->str) && node->type == DOUBLE_QUOTED || node->type == UNQUOTED)
		{
			node->str = expand_split(node->str, s_env);
			if (old_value != NULL)
				free(old_value);
		}
		node = node->next;
	}
}

int	has_var(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

char	*expand_split(char *s, t_env *s_env)
{
	int		flag1;
	int		flag2;
	int		count;
	char	**split;
	char	*ret = NULL;

	flag1 = 0;
	flag2 = 0;
	if (s[0] != '$')
		flag1 = 1;
	if (s[ft_strlen(s) - 1] == '$')
		flag2 = 1;
	split = ft_split(s, "$");
	count = count_split(split);
	expand_words(split, s_env, flag1, flag2);
	int	j = 0;
	while (j < count)
		printf("[%s]\n", split[j++]);
	ret = join_words(split, count);
	free_split(split);
	return (ret);
}

static int	count_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

static void	expand_words(char **split, t_env *s_env, int flag1, int flag2)
{
	int		i;
	char	*old;

	i = 0;
	old = NULL;
	while (split[i])
	{
		if (flag2 == 1 && i == count_split(split) - 1)
		{
			flag2 = 0;
			split[i] = add_dollar_end(&split[i]);
		}
		if (flag1 == 1)
		{
			flag1 = 0;
			i++;
			continue ;
		}
		if (is_delimiter(split[i][0]) == 1)
			split[i] = add_dollar(&split[i]);
		else if (lookup_var(s_env, split[i]) == 1)
		{
			old = split[i];										// make all these free lines into 2 lines later
			split[i] = expand(split[i], s_env);
			if (old != NULL)
				free(old);
		}
		else
		{
			old = split[i];
			split[i] = handle_unknown_var(split[i]);
			if (old != NULL)
				free(old);
		}
		i++;
	}
}

static char	*add_dollar_end(char **s)
{
	char	*ret;
	int		len;
	int		i;

	ret = malloc(sizeof(char) * (ft_strlen(*s) + 2));
	if (!ret)
		return (NULL);
	len = ft_strlen(*s);
	i = 0;
	while ((*s)[i] && i < len)
	{
		ret[i] = (*s)[i];
		i++;
	}
	ret[i] = '$';
	i++;
	ret[i] = '\0';
	free(*s);
	return (ret);
}

static int	is_delimiter(char c)
{
	return (c == ' ' || c == '\'' || c == '\"' || c == '\0');
}

static char	*add_dollar(char **s)
{
	char	*ret;
	int		i;

	ret = malloc(sizeof(char) * (ft_strlen(*s) + 2));
	if (!ret)
		return (NULL);
	ret[0] = '$';
	i = 1;
	while ((*s)[i - 1])
	{
		ret[i] = (*s)[i - 1];
		i++;
	}
	ret[i] = '\0';
	free(*s);
	return (ret);
}

static int	lookup_var(t_env *s_env, char *name)
{
	t_env	*node;

	node = s_env;
	while (node)
	{
		if (ft_strcmp_env(node->name, name) == 0)
			return (1);
		node = node->next;
	}
	return (0);
}

static char	*expand(char *s, t_env *s_env)
{
	char	*space;
	char	*env_value;

	space = get_space_start(&s);
	env_value = get_env_value2(s_env, s);
	if (!env_value)
		return (ft_strdup(""));
	return (copy_env_with_space(env_value, space));
}

static char	*handle_unknown_var(char *s)
{
	int		i;
	char	*space;
	char	*ret;

	space = get_space_start(&s);
	if (!space)
		return (ft_strdup(""));
	ret = malloc(sizeof(char) * (ft_strlen(space) + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (space[i])
	{
		ret[i] = space[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

static char	*copy_env_with_space(const char *env_value, const char *space)
{
	int		env_len = ft_strlen(env_value);
	int		space_len = ft_strlen(space);
	char	*ret = malloc(env_len + space_len + 1);
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!ret)
		return (NULL);
	while (i < env_len)
		ret[i++] = env_value[j++];
	if (space)
	{
		while (*space)
			ret[i++] = *space++;
	}
	ret[i] = '\0';
	return (ret);
}

static char	*get_space_start(char **s)
{
	char	*ret;
	int		i;

	ret = *s;
	i = 0;
	while (ret[i] && (ret[i] != ' ' && ret[i] != '\''))
		i++;
	if (ret[i] == '\0')
		return (NULL);
	return (ret + i);
}

static char *join_words(char **split, int count)
{
	int		i;
	char	*ret;
	char	*old;

	i = 0;
	ret = NULL;
	old = NULL;
	while (count--)
	{
		if (ret == NULL)
			ret = ft_strdup(split[i]);
		else
		{
			old = ret;
			if (!split[i])
				split[i] = handle_pid(&split[i]);
			ret = ft_strconcat(ret, split[i]);
			free(old);
		}
		i++;
	}
	return (ret);
}

static char	*handle_pid(char **s)
{
	char	*ret;

	ret = getenv("$$");
	if (!ret)
		return (ft_strdup("$$"));
	if (*s)
		free(*s);
	return (ret);
}

static void	free_split(char **split)			//add count to free last elements
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}*/
