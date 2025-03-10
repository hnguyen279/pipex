/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 13:42:32 by thi-huon          #+#    #+#             */
/*   Updated: 2025/02/21 14:38:01 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_exit_error(char *msg)
{
	perror(msg);
	exit(1);
}
// void display_error(char *msg, char *detail, int exit_code)
// {
//     ft_printf_fd(2, "%s: %s\n", msg, detail);
//     exit(exit_code);
// }


void	display_error(char *prefix, char *msg)
{
	while (*prefix)
		write(2, prefix++, 1);
	write(2, ": ", 2);
	while (*msg)
		write(2, msg++, 1);
	write(2, "\n", 1);
}

void	free_split(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	close_fds(int fd1, int fd2, int fd3)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	if (fd3 >= 0)
		close(fd3);
}

int	open_file(char *file, int *pipe_fd, int check_stdin_stdout)
{
    int fd;

	if (check_stdin_stdout == 0)
		fd = open(file, O_RDONLY);
	if (check_stdin_stdout == 1)
		fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fd == -1)
	{
		close_fds(pipe_fd[0], pipe_fd[1], -1);
		check_exit_error("open failed");
	}
	return (fd);
}
