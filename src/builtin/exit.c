/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:13:32 by hnemmass          #+#    #+#             */
/*   Updated: 2025/05/22 20:35:12 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

int	check_is_number(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] == ' ' || cmd[i] == '\t')
		i++;
	if (cmd[i] == '+' || cmd[i] == '-')
		i++;
	if (!cmd[i])
		return (1);
	while (cmd[i])
	{
		if (cmd[i] < '0' || cmd[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	exit_nonnum(char *cmd)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(2);
}

long long	ft_atoi_strict(char *cmd)
{
	int			i;
	long long	num;
	int			sign;
	long long	checker;

	i = 0;
	num = 0;
	sign = 1;
	while (cmd[i] == ' ' || cmd[i] == '\t')
		i++;
	if (cmd[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (cmd[i] == '+')
		i++;
	while (cmd[i] >= '0' && cmd[i] <= '9')
	{
        checker = num;
		if (num == 922337203685477580 && ((sign == 1 && cmd[i] > '7')
			|| (sign == -1 && cmd[i] > '8')))
			exit_nonnum(cmd);
        if (num == 922337203685477580 && cmd[i] == '8' && !cmd[i + 1] && sign == -1)
            return (0);
        num = num * 10 + (cmd[i] - '0');
        if (num / 10 != checker)
            exit_nonnum(cmd);
        i++;
	}
	return (num * sign);
}

int ft_exit(char **cmd, t_minishell *mini)
{
	long long		num;

	ft_putstr_fd("exit\n", 2);
	if (!cmd[1])
		exit (mini->exit_status);
	if (check_is_number(cmd[1]))
		exit_nonnum(cmd[1]);
	if (cmd[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	num = ft_atoi_strict(cmd[1]);
	exit(num % 256);
}