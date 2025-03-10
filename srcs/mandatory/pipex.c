/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:21:12 by thi-huon          #+#    #+#             */
/*   Updated: 2025/02/21 15:39:52 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_child_process(char **argv, char **env, int *pipe_fd)
{
	int	fd_in;

	fd_in = open_file(argv[1], pipe_fd, 0);
	if (dup2(fd_in, 0) == -1)
	{
		close_fds(fd_in, -1, -1);
		check_exit_error("dup2 failed");
	}
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		close_fds(fd_in, pipe_fd[0], pipe_fd[1]);
		check_exit_error("dup2 failed");
	}
	close_fds(fd_in, pipe_fd[0], pipe_fd[1]);
	exec_cmd(argv[2], env);
	exit(1);
}

void	second_child_process(char **argv, char **env, int *pipe_fd)
{
	int	fd_out;

	fd_out = open_file(argv[4], pipe_fd, 1);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close_fds(fd_out, pipe_fd[0], pipe_fd[1]);
		check_exit_error("dup2 failed");
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		close_fds(fd_out, -1, -1);
		check_exit_error("dup2 failed");
	}
	close_fds(fd_out, pipe_fd[0], pipe_fd[1]);
	exec_cmd(argv[3], env);
	exit(1);
}
