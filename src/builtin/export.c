/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:13:32 by hnemmass          #+#    #+#             */
/*   Updated: 2025/05/08 14:35:40 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *ft_strjoin(char *s1, char *s2)
{
    char    *result;
    size_t  len1;
    size_t  len2;
    size_t  i;
    size_t  j;
    
    if (!s1 && !s2)
        return (NULL);
    if (!s1)
        return (ft_strdup(s2));
    if (!s2)
        return (ft_strdup(s1));
        
    len1 = strlen(s1);
    len2 = strlen(s2);
    result = malloc(len1 + len2 + 1);
    if (!result)
        return (NULL);
        
    i = 0;
    while (i < len1)
    {
        result[i] = s1[i];
        i++;
    }
    
    j = 0;
    while (j < len2)
    {
        result[i + j] = s2[j];
        j++;
    }
    result[i + j] = '\0';
    return (result);
}

void	free_copy(t_env *copy)
{
	t_env	*current;

	while(copy)
	{
		current = copy;
		copy = copy->next;
		free(current->name);
		if (current->value)
			free(current->value);
		free(current);
	}
}

int	check_if_sorted(t_env *env)
{
	t_env	*current;
	int		i;

	current = env;
	while (current && current->next)
	{
		i = 0;
		while (current->name[i] == current->next->name[i] && (current->name[i] || current->next->name[i]))
			i++;
		if (current->name[i] > current->next->name[i])
			return (1);
		current = current->next;
	}
	return (0);
}
t_env	*create_copy(t_env *env)
{
	t_env	*current;
	t_env	*copy;
	t_env	*new_node;
	t_env	*last;

	copy = NULL;
	last = NULL;
	current = env;
	while (current)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->name = ft_strdup(current->name);
		if (current->value)
			new_node->value = ft_strdup(current->value);
		else
			new_node->value = NULL;
		new_node->next = NULL;
		if (!copy)
			copy = new_node;
		else
			last->next = new_node;
		last = new_node;
		current = current->next;
	}
	return (copy);
}

void sort_copy(t_env *copy)
{
	t_env	*current;
	char	*tmp_name;
	char	*tmp_value;
	int		i;
	
	if (!copy || !check_if_sorted(copy))
		return ;
	current = copy;
	while (current && current->next)
	{
		i = 0;
		while (current->name[i] == current->next->name[i] && (current->name[i] || current->next->name[i]))
			i++;
		if (current->name[i] > current->next->name[i])
		{
			tmp_name = current->name;
			tmp_value = current->value;
			current->name = current->next->name;
			current->value = current->next->value;
			current->next->name = tmp_name;
			current->next->value = tmp_value;
		}
		current = current->next;
	}
	if (check_if_sorted(copy))
		sort_copy(copy);
}

void sort_and_display(t_env *env)
{
	t_env	*copy;
	t_env	*current;
	
	copy = create_copy(env);
	if (!copy)
		return ;
	sort_copy(copy);
	current = copy;
	while (current)
	{
		if ((current->name[0] == '_') && !current->name[1])
		{
			if (current->next)
			{
				current = current->next;
				continue;
			}
			else
				break ;
		}
		else if (current->value)
			printf("declare -x %s=\"%s\"\n", current->name, current->value);
		else
			printf("declare -x %s\n", current->name);
		current = current->next;
	}
	free_copy(copy);
}

int check_valid_input(char *cmd)
{
    int i = 0;
    
    if (!cmd[0])
        return (1);
	if ((cmd[0] == '_') && (cmd[1] == '=' || !cmd[1]))
		return (5);
    if ((cmd[0] != '_') && 
        (cmd[0] < 'a' || cmd[0] > 'z') && 
        (cmd[0] < 'A' || cmd[0] > 'Z'))
        return (1);
    while (cmd[i] && cmd[i] != '=' && !(cmd[i] == '+' && cmd[i+1] == '='))
    {
        if (i > 0 && (cmd[i] < 'a' || cmd[i] > 'z') && 
            (cmd[i] < 'A' || cmd[i] > 'Z') && 
            (cmd[i] < '0' || cmd[i] > '9') && 
            cmd[i] != '_')
            return (1);
        i++;
    }
    if (i == 0)
        return (1);
    if (cmd[i] == '\0')
        return (2);
    else if (cmd[i] == '=')
        return (3);
    else if (cmd[i] == '+' && cmd[i+1] == '=')
        return (4);
    return (0);
}

t_env	*ft_lstnew(char *name, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node -> name = name;
	node -> value = value;
	node -> next = NULL;
	return (node);
}

void	ft_lstadd_back(t_env *env, t_env *new)
{
	t_env	*temp;

	if (!new || !env)
		return ;
	if (!env->name && !env->value)
	{
		env = new;
		return ;
	}
	temp = env;
	while (temp -> next != NULL)
		temp = temp -> next;
	temp -> next = new;
}

char **modified_split(char *cmd, int flag)
{
    int     i;
    char    **name_value;
    
    name_value = malloc(2 * sizeof(char *));
    if (!name_value)
        return (NULL);
    i = 0;
    if (flag == 3)
    {
        while (cmd[i] && cmd[i] != '=')
            i++;
        name_value[0] = ft_strndup(cmd, i);
        name_value[1] = ft_strdup(cmd + i + 1);
    }
    else if (flag == 4)
    {
        while (cmd[i] && !(cmd[i] == '+' && cmd[i+1] == '='))
            i++;
        name_value[0] = ft_strndup(cmd, i);
        name_value[1] = ft_strdup(cmd + i + 2);
    }
    if (!name_value[0] || !name_value[1])
    {
        if (name_value[0])
            free(name_value[0]);
        if (name_value[1])
            free(name_value[1]);
        free(name_value);
        return (NULL);
    }
    return (name_value);
}

t_env *find_env_var(t_env *env, char *name)
{
    t_env *current;
    
    current = env;
    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
            return (current);
        current = current->next;
    }
    return (NULL);
}

static void	handle_export_new_var(char *name, t_env *env)
{
	t_env	*new_node;
	
	new_node = ft_lstnew(ft_strdup(name), NULL);
	if (new_node)
		ft_lstadd_back(env, new_node);
}

static void	handle_existing_var_append(t_env *existing, char **name_value)
{
	char	*tmp;
	
	if (existing->value)
	{
		tmp = ft_strjoin(existing->value, name_value[1]);
		free(existing->value);
		existing->value = tmp;
		free(name_value[0]);
		free(name_value[1]);
	}
	else
	{
		existing->value = name_value[1];
		free(name_value[0]);
	}
}

static void	handle_non_existing_var(char **name_value, t_env *env)
{
	t_env	*new_node;
	
	new_node = ft_lstnew(name_value[0], name_value[1]);
	if (new_node)
		ft_lstadd_back(env, new_node);
	else
	{
		free(name_value[0]);
		free(name_value[1]);
	}
}

static void	handle_variable_with_value(char *cmd, int flag, t_env *env)
{
	char	**name_value;
	t_env	*existing;
	
	name_value = modified_split(cmd, flag);
	if (!name_value)
		return ;
	existing = find_env_var(env, name_value[0]);
	if (existing)
	{
		if (flag == 4 && existing->value)
			handle_existing_var_append(existing, name_value);
		else
		{
			free(existing->value);
			existing->value = name_value[1];
			free(name_value[0]);
		}
	}
	else
		handle_non_existing_var(name_value, env);
	free(name_value);
}

int	ft_export(char **cmd, t_env *env)
{
	int		i;
	int		flag;

	i = 1;
	if (!cmd[1])
		sort_and_display(env);
	while (cmd[i])
	{
		flag = check_valid_input(cmd[i]);
		if (flag == 5)
		{
			i++;
			continue ;
		}
		if (flag == 1)
		{
			printf("export: `%s': not a valid identifier\n", cmd[i]);
			i++;
			continue ;
		}
		if (flag == 2)
		{
			if (!find_env_var(env, cmd[i]))
				handle_export_new_var(cmd[i], env);
		}
		else if (flag == 3 || flag == 4)
			handle_variable_with_value(cmd[i], flag, env);
		i++;
	}
	return (0);
}
