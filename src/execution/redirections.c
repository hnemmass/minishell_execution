/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:05:26 by hnemmass          #+#    #+#             */
/*   Updated: 2025/05/06 18:32:02 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write (fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s == NULL || fd < 0)
		return ;
	while (*s != '\0')
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
}

static int	compare(char *line, char *delimiter)
{
	int	i;

	i = 0;
	if (!line && !delimiter)
		return (1);
	while (line[i] && delimiter[i] && line[i] == delimiter[i])
		i++;
	if (line[i] == '\n' && !delimiter[i])
		return (0);
	return (1);
}

static int	handle_heredoc(char *delimiter, int position)
{
	char	*line;
	int		temp_fd;
	
	temp_fd = open("heredoc_tmp", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (temp_fd == -1)
		return (perror("heredoc"), 1);
	while (1337)
	{
		ft_putstr_fd("> ", 2);
		line = get_next_line(0);
		if (!line || !compare(line, delimiter))
		{
			free(line);
			break;
		}
		ft_putstr_fd(line, temp_fd);
		free(line);
	}
	close(temp_fd);
	if (position == 0)
	{
		temp_fd = open("heredoc_tmp", O_RDONLY);
		if (temp_fd == -1)
			return (perror("heredoc"), 1);
		dup2(temp_fd, STDIN_FILENO);
		close(temp_fd);
		unlink("heredoc_tmp");
	}
	return (0);
}

static int	open_with_mode(char *filename, int mode)
{
	int fd;
	
	if (mode == 1)
		fd = open(filename, O_RDONLY);
	else if (mode == 2)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (mode == 3)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	// else
	// 	return (-1);
	if (fd == -1)
		perror(filename);
	return (fd);
}

static int	get_fd_type(int token_type)
{
	if (token_type == TOKEN_RED_IN)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

int	apply_redirections(t_redirect *red, int hdoc_position)
{
	int fd;
	int mode;
	
	if (red->type == TOKEN_RED_IN)
		mode = 1;
	else if (red->type == TOKEN_RED_OUT)
		mode = 2;
	else if (red->type == TOKEN_APPEND)
		mode = 3;
	else if (red->type == TOKEN_HDOC)
	{
		return (handle_heredoc(red->file, hdoc_position));
	}
	else
		return (0);
	fd = open_with_mode(red->file, mode);
	if (fd == -1)
		exit(1);
	dup2(fd, get_fd_type(red->type));
	close(fd);

	return (0);
}
