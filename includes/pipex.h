/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 14:02:35 by thi-huon          #+#    #+#             */
/*   Updated: 2025/02/21 14:15:43 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "lib.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>

void	check_exit_error(char *msg);
void	display_error(char *prefix, char *msg);
//void    display_error(char *msg, char *detail, int exit_code);
void	free_split(char **tab);
void	close_fds(int fd1, int fd2, int fd3);
int		open_file(char *file,int *pipe_fd, int check_stdin_stdout);
void	exec_cmd(char *cmd, char **env);
void	first_child_process(char **argv, char **env, int *pipe_fd);
void	second_child_process(char **argv, char **env, int *pipe_fd);

#endif
