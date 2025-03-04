#ifndef PIPEX_H
#define PIPEX_H

#include "lib.h" 
#include <sys/wait.h>
# include <sys/types.h>
# include <stdio.h>
# include <fcntl.h>  


void check_exit_error(char *msg);
void display_error(char *prefix, char *msg);
void	free_split(char **tab);
void close_fds(int fd1, int fd2, int fd3);
int open_file(char *file, int check_stdin_stdout);
void exec_cmd(char *cmd, char **env);
void child_process(char **argv, char **env, int *pile_fd);
void parent_process(char **argv, char **env, int *pile_fd);



#endif
