/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:13:32 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/26 13:40:22 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef struct s_env {
// 	char *name;
// 	char *value;
// 	struct s_env *next;
// } t_env;

// // Include all your functions above here...

// int	ft_strcmp(char *s1, char *s2)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!s1)
// 		return (1);
// 	while (s1[i] || s2[i])
// 	{
// 		if (s1[i] != s2[i])
// 			return (s1[i] - s2[i]);
// 		i++;
// 	}
// 	return (0);
// }

// // Helper to create a new env node
// t_env *new_node(char *name, char *value) {
// 	t_env *node = malloc(sizeof(t_env));
// 	if (!node) return NULL;
// 	node->name = strdup(name);
// 	node->value = value ? strdup(value) : NULL;
// 	node->next = NULL;
// 	return node;
// }

// // Helper to append a node to a list
// void append_node(t_env **head, t_env *new) {
// 	if (!*head)
// 		*head = new;
// 	else {
// 		t_env *curr = *head;
// 		while (curr->next)
// 			curr = curr->next;
// 		curr->next = new;
// 	}
// }

// // Free the original env list (not the copy)
// void free_env(t_env *env) {
// 	t_env *tmp;
// 	while (env) {
// 		tmp = env;
// 		env = env->next;
// 		free(tmp->name);
// 		if (tmp->value)
// 			free(tmp->value);
// 		free(tmp);
// 	}
// }

char	*ft_strdup(char *s)
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
}

char *ft_strndup(const char *s, size_t n)
{
    char *dup;
    size_t i;
    
    dup = malloc(n + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (i < n && s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}

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
		if (current->value)
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

void	ft_export(char **cmd, t_env *env)
{
	int		i;
	int 	flag;
	t_env	*new_node;
	t_env	*existing;
	char	**name_value;
	char	*tmp;

	i = 1;
	if(!cmd[1])
		sort_and_display(env);
	while (cmd[i])
	{
		flag = check_valid_input(cmd[i]);
		if (flag == 1)
		{
			printf("export: `%s': not a valid identifier\n", cmd[i]);
			i++;
			continue ;
		}
		if (flag == 2)
		{
			existing = find_env_var(env, cmd[i]);
			if(!existing)
			{
				new_node = ft_lstnew(ft_strdup(cmd[i]), NULL);
                if (new_node)
                    ft_lstadd_back(env, new_node);
			}
		}
        else if (flag == 3 || flag == 4)
        {
            name_value = modified_split(cmd[i], flag);
            if (!name_value)
            {
                i++;
                continue;
            }
            existing = find_env_var(env, name_value[0]);
            if (existing)
            {
                if (flag == 4 && existing->value)
                {
                    tmp = ft_strjoin(existing->value, name_value[1]);
                    free(existing->value);
                    existing->value = tmp;
                    free(name_value[0]);
                    free(name_value[1]);
                }
                else
                {
                    free(existing->value);
                    existing->value = name_value[1];
                    free(name_value[0]);
                }
            }
            else
            {
                new_node = ft_lstnew(name_value[0], name_value[1]);
                if (new_node)
                    ft_lstadd_back(env, new_node);
                else
                {
                    free(name_value[0]);
                    free(name_value[1]);
                }
            }
            free(name_value);
        }
        i++;
	}
}

// Declare or include your functions and structs here
// (e.g., t_env, export, new_node, append_node, etc.)
// Assume your export.c content is compiled with this

// int main(void)
// {
// 	t_env *env = NULL;

// 	// --- Prepopulate environment ---
// 	append_node(&env, new_node("EMPTY", NULL));
// 	append_node(&env, new_node("__X", "secret2"));
// 	append_node(&env, new_node("_X", "secret"));
// 	append_node(&env, new_node("__x", "secret2"));
// 	append_node(&env, new_node("_x", "public"));
// 	append_node(&env, new_node("X1", "val1"));
// 	append_node(&env, new_node("X10", "val10"));
// 	append_node(&env, new_node("X2", "val2"));
// 	append_node(&env, new_node("X", "===kmlk"));
// 	append_node(&env, new_node("LONGVARXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", 
// 		"LONGVALUEYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"));
// 	append_node(&env, new_node("Q", "quick"));
// 	append_node(&env, new_node("a", "lowercase"));
// 	append_node(&env, new_node("A", "uppercase"));

// 	// --- Begin Tests ---
	
// 	// Test 1: export with no arguments (should print sorted env)
// 	char *cmd1[] = {"export", NULL};
// 	printf("=== Test 1: export with prepopulated env ===\n");
// 	export(cmd1, env);

// 	// Test 2: Valid variable declarations
// 	printf("\n=== Test 2: Valid exports ===\n");
// 	char *cmd2[] = {"export", "FOO=bar", "BAR=baz", "EMPTY2=", "ONLYNAME", NULL};
// 	export(cmd2, env);


// 	// Test 3: Invalid identifiers
// 	printf("\n=== Test 3: Invalid identifiers ===\n");
// 	char *cmd3[] = {"export", "1INVALID=bad", "=equals", "VA@R=oops", "++=fail", "ONLYNAME2", NULL};
// 	export(cmd3, env);

// 	// Test 4: Appending to existing and non-existing variables
// 	printf("\n=== Test 4: Appending values ===\n");
// 	char *cmd4[] = {"export", "FOO+=123", "NEWVAR+=value", NULL};
// 	export(cmd4, env);

// 	// Test 5: Reassigning existing variable
// 	printf("\n=== Test 5: Reassign existing variable ===\n");
// 	char *cmd5[] = {"export", "FOO=newvalue", NULL};
// 	export(cmd5, env);
// 	char *cmd10[] = {"export", NULL};
// 	printf("=== Test 2: export with prepopulated env ===\n");
// 	export(cmd10, env);

// 	// Test 6: export with no arguments (final sorted list)
// 	printf("\n=== Test 6: Final sorted export ===\n");
// 	export(cmd1, env);

// 	// Test 7: Long name and value again
// 	printf("\n=== Test 7: Long names and values (again) ===\n");
// 	char *cmd7[] = {"export", 
// 		"LONGVARXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX2=LONGVALUEZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ", 
// 		NULL};
// 	export(cmd7, env);

// 	// Test 8: Mixed casing and underscore (more)
// 	printf("\n=== Test 8: Mixed casing and underscore ===\n");
// 	char *cmd8[] = {"export", "b__=secon_/dlower", "B=SECONDUPPER", NULL};
// 	export(cmd8, env);

// 	// Final output again
// 	printf("\n=== Final Sorted Export (after all operations) ===\n");
// 	export(cmd1, env);

// 	// Cleanup
// 	free_env(env);
// 	return 0;
// }
