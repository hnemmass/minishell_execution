/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:02:42 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/23 17:25:31 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

int	ft_echo(char **cmd)
{
	char	**current;
	int		i;

	i = 1;
	current = cmd;
	if (ft_strcmp(current[i], "-n") == 0)
	{
		i++;
		while (ft_strcmp(current[i], "-n") == 0)
			i++;
		while (current[i + 1])
		{
			printf("%s ", current[i]);
			i++;
		}
		printf("%s", current[i]);
		return (0);
	}
	else
	{
		while (current[i + 1])
		{
			printf("%s ", current[i]);
			i++;
		}
		printf("%s\n", current[i]);
		return (0);
	}
	return (1);
}
