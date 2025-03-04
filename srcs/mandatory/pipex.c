
#include "pipex.h"

void child_process(char **argv, char **env, int *pile_fd)
{
    int fd_in;

    fd_in = open_file(argv[1], STDIN_FILENO); // 0;
    if (dup2(fd_in, 0) == -1)
    {
        close_fds(fd_in, -1, -1);
        check_exit_error("dup2 failed");
    }
    if (dup2(pile_fd[1], STDOUT_FILENO) == -1) //1
    {
		close_fds(fd_in, pile_fd[0], pile_fd[1]);
        check_exit_error("dup2 failed");
    }
	close_fds(fd_in, pile_fd[0], pile_fd[1]);
    exec_cmd(argv[2], env);
}

void parent_process(char **argv, char **env, int *pile_fd)
{
    int fd_out;

    fd_out = open_file(argv[4], 1);
	
	if (dup2(pile_fd[0], STDIN_FILENO) == -1) //0
    {
		close_fds(fd_out, pile_fd[0], pile_fd[1]);
        check_exit_error("dup2 failed");
    }
	if (dup2(fd_out, STDOUT_FILENO) == -1) //1
    {
        close_fds(fd_out, -1, -1);
        check_exit_error("dup2 failed");
    }
	close_fds(fd_out, pile_fd[0], pile_fd[1]);
    exec_cmd(argv[3], env);
}
