/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 12:47:55 by thi-huon          #+#    #+#             */
/*   Updated: 2025/02/21 12:48:00 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// int	main(int argc, char **argv, char **env)
// {
// 	int		pipe_fd[2];
// 	pid_t	pid;

// 	if (argc != 5)
// 	{
// 		display_error("Input format", "./pipex file1 cmd1 cmd2 file2");
// 		exit(1);
// 	}
// 	if (pipe(pipe_fd) == -1)
// 		check_exit_error("pipe failed");
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		close_fds(pipe_fd[0], pipe_fd[1], -1);
// 		check_exit_error("fork failed");
// 	}
// 	if (pid == 0)
// 		child_process(argv, env, pipe_fd);
// 	else
// 	{
// 		if (wait(NULL) == -1)
// 			check_exit_error("waitpid failed");
// 		parent_process(argv, env, pipe_fd);
// 	}
// }

static void handle_fork_error(int fd1, int fd2, int fd3, char *msg)
{
    close_fds(fd1, fd2, fd3);  
    check_exit_error(msg);
}

static int wait_for_children(pid_t pid1, pid_t pid2)
{
    int status1;
    int status2;

    if (waitpid(pid1, &status1, 0) == -1)
        check_exit_error("waitpid failed for first child");

    if (waitpid(pid2, &status2, 0) == -1)
        check_exit_error("waitpid failed for second child");

    if (WIFEXITED(status2))
        return WEXITSTATUS(status2);
    else if (WIFSIGNALED(status2))
        return (128 + WTERMSIG(status2));

    return (1);
}

int main(int argc, char **argv, char **env)
{
    int pipe_fd[2];
    pid_t pid1;
    pid_t pid2;
  
    if (argc != 5)
    {
        display_error("Input format", "./pipex file1 cmd1 cmd2 file2");
        exit(1); 
    }
    if (pipe(pipe_fd) == -1)
        check_exit_error("pipe failed");
    pid1 = fork(); 
    if (pid1 == -1)
        handle_fork_error(pipe_fd[0], pipe_fd[1], -1, "fork failed");
    if (pid1 == 0)
        first_child_process(argv, env, pipe_fd);
    pid2 = fork();
    if (pid2 == -1)
        handle_fork_error(pipe_fd[0], pipe_fd[1], -1, "fork failed");
    if (pid2 == 0)
        second_child_process(argv, env, pipe_fd);
    close_fds(pipe_fd[0], pipe_fd[1], -1);
    return (wait_for_children(pid1, pid2)); 
}
