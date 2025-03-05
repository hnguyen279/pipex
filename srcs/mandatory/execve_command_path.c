/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_command_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 14:52:12 by thi-huon          #+#    #+#             */
/*   Updated: 2025/02/21 14:55:21 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*find_path(char **env)
{
	int	i;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			if (env[i][5] != '\0')
				return (env[i] + 5);
			else
				return (NULL);
		}
		i++;
	}
	return (NULL);
}

static char	*build_cmd_path(char *first_cmd, char *f_path)
{
	char	**env_path;
	char	*one_path;
	char	*cmd_path;
	int		i;

	i = 0;
	env_path = ft_split(f_path, ':');
	if (!env_path)
		return (NULL);
	while (env_path[i])
	{
		one_path = ft_strjoin(env_path[i], "/");
		cmd_path = ft_strjoin(one_path, first_cmd);
		free(one_path);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_split(env_path);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_split(env_path);
	return (NULL);
}

static char *get_path(char *first_cmd, char **env)
{
	char	*f_path;
    char	*cmd_path;

    if (!first_cmd || first_cmd[0] == '\0')
    {
        display_error("Error", "command not found");
        return (NULL);
    }
    f_path = find_path(env);
    if (!f_path)
    {
        display_error("Error", "PATH environment variable not valid");
        return (NULL);
    }
    cmd_path = build_cmd_path(first_cmd, f_path);
    if (!cmd_path)
    {
        display_error(first_cmd, "command not found");
        return (NULL);
    }
    return (cmd_path);
}

void	exec_cmd(char *cmd, char **env)
{
	char    **split_cmd;
	char    *cmd_path;

	if (!cmd || !env)
        exit(127);
	split_cmd = ft_split(cmd, ' ');
	if (!split_cmd)
		exit(1); 
	cmd_path = get_path(split_cmd[0], env);
	if (!cmd_path)
	{
		free_split(split_cmd);
		exit(127);
	}
	if (execve(cmd_path, split_cmd, env) == -1)
	{
		free_split(split_cmd);
		free(cmd_path);
		check_exit_error("execve failed");
	}
}
