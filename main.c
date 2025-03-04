
#include "pipex.h"
//lll

int main(int argc, char **argv, char **env)
{
    int pile_fd[2];
    pid_t pid;

    if (argc != 5)
    {
        display_error("Input format", "./pipex file1 cmd1 cmd2 file2");
        exit(EXIT_FAILURE);
    }
    if (pipe(pile_fd) == -1)
        check_exit_error("pipe failed");
    pid = fork();
    if (pid == -1)
	{
		close_fds(pile_fd[0], pile_fd[1], -1);
		check_exit_error("fork failed");
	}
    if (pid == 0)
        child_process(argv, env, pile_fd);
    else
    {
        if (wait(NULL) == -1) //only one child process use wait for simple or can use waitpid(pid, NULL, 0) == -1;
			check_exit_error("waitpid failed");
        parent_process(argv, env, pile_fd);
    }
}
