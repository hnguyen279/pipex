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
		if (!one_path || !cmd_path)
		{
			free_split(env_path);
			return (NULL);
		}
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

    // if (!first_cmd || first_cmd[0] == '\0')
    // {
    //     display_error("Error", "command not found");
    //     return (NULL);
    // }
    f_path = find_path(env);
    if (!f_path)
    {
        ft_printf_fd(2, "No such file or directory (PATH not valid)\n");
        //display_error("Error", "No such file or directory (PATH not valid)");
        exit(127);
    }
    cmd_path = build_cmd_path(first_cmd, f_path);
    if (!cmd_path)
    {
        ft_printf_fd(2, "%s: command not found\n", first_cmd);
        //display_error(first_cmd, "command not found");
        return (NULL);
    }
    return (cmd_path);
}


int check_white_spaces_cmd(char *cmd)
{
    int i = 0;
    while (cmd[i])
    {
        if (cmd[i] != ' ' && (cmd[i] < 9 || cmd[i] > 13))
            return 0;
        i++;
    }
    return 1;
}

int skip_unquoted_text(char *cmd, int i) // echo hello\\ world; find echo
{
    while (cmd[i] && cmd[i] != ' ' &&
		cmd[i] != '\"' && cmd[i] != '\'')
    {
        if (cmd[i] == '\\' && cmd[i + 1] != '\0')
            i += 2;
        else
            i++;
    }
    return i;
}

// int skip_quoted_text(char *cmd, int i)
// {
//     char quote = cmd[i];
//     i++; 
//     while (cmd[i] && cmd[i] != quote)
//         i++;
//     if (cmd[i] != quote)
//     {
//         ft_printf_fd(2, "Missing close quote: %c\n", quote);
//         exit(1);
//     }
//     return i;
// }

int skip_quoted_text(char *cmd, int i) 
{
    char quote = cmd[i];
    i++; 
    while (cmd[i])
    {
        if (cmd[i] == quote)
            break;
        i++;
    }
    if (cmd[i] == '\0')
    {
        ft_printf_fd(2, "Missing close quote: %c\n", quote);
        exit(1);
    }
    return i;
}


// static char *extract_token(char *cmd, int len)
// {
//     char *token;
//     int i;
//     int j;

//     i = 0;
//     j = 0;
//     token = (char *)malloc(sizeof(char) * (len + 1));
//     if (!token)
//         return NULL;
//     while (i < len)
//     {
//         if ((cmd[0] == '\"' || cmd[0] == '\'') && cmd[i] == cmd[0])
//             i++;
//         else if (cmd[0] != '\"' && cmd[0] != '\'' && cmd[i] == ' ')
//             i++;
//         else
//         {
//             if (cmd[0] != '\"' && cmd[0] != '\'' && cmd[i] == '\\')
//                 i++;
//             token[j++] = cmd[i++];
//         }
//     }
//     token[j] = '\0';
//     return token;
// }

static char *extract_token(char *cmd, int len)
{
    char *token;
    int i;
    int j;

    i = 0;
    j = 0;
    token = (char *)malloc(sizeof(char) * (len + 1));
    if (!token)
        return NULL;
    while (i < len)
    {
        if ((cmd[0] == '\"' || cmd[0] == '\'') && cmd[i] == cmd[0])
            i++; 
        else if (cmd[0] != '\"' && cmd[0] != '\'' && cmd[i] == ' ')
            i++; 
        else if (cmd[0] != '\"' && cmd[0] != '\'' && cmd[i] == '\\')
        {
            i++; 
            token[j++] = cmd[i++]; 
        }
        else
            token[j++] = cmd[i++]; 
    }
    token[j] = '\0';
    return token;
}

static char *get_token(char *cmd, int *pos)
{
    int start;
    char quote;
    int end;

    while (cmd[*pos] == ' ') 
        (*pos)++;
    if (cmd[*pos] == '\0') 
        return NULL;
    start = *pos;
    if (cmd[*pos] == '\"' || cmd[*pos] == '\'') 
    {
        quote = cmd[(*pos)++];
        while (cmd[*pos] && cmd[*pos] != quote)
            (*pos)++;
        if (cmd[*pos] == quote)
            (*pos)++; 
    }
    else 
        *pos = skip_unquoted_text(cmd, *pos);
    end = *pos;
    return extract_token(cmd + start, end - start);
}


// static char **split_tokens(char *cmd, char **tokens, int total_tokens)
// {
//     int tokenIndex;
//     int pos;

//     tokenIndex = 0;
//     pos = 0;
//     while (tokenIndex < total_tokens)
//     {
//         tokens[tokenIndex] = get_token(cmd, &pos);
//         if (!tokens[tokenIndex])
//         {
//             free_split(tokens);
//             return NULL;
//         }
//         tokenIndex++;
//     }
//     return tokens;
// }

// static int count_tokens(char *cmd)
// {
//     int tokens = 0;
//     int i = 0;
//     while (cmd[i])
//     {
//         if (cmd[i] == '\"' || cmd[i] == '\'')
//         {
//             tokens++;
//             i = skip_quoted_text(cmd, i) + 1;
//         }
//         else if (cmd[i] != ' ')
//         {
//             tokens++;
//             i = skip_unquoted_text(cmd, i);
//         }
//         else
//             i++;
//     }
//     return tokens;
// }


// char **split_command(char *cmd)
// {
//     int total_tokens;
//     char **tokens;

//     total_tokens = count_tokens(cmd);
//     tokens = (char **)ft_calloc(total_tokens + 1, sizeof(char *));
//     if (!tokens)
//         return NULL;
//     tokens = split_tokens(cmd, tokens, total_tokens);
//     return tokens;
// }

static int count_tokens(char *cmd)
{
    int tokens = 0;
    int i = 0;

    while (cmd[i])
    {
        if (cmd[i] != ' ')
        {
            tokens++;
            if (cmd[i] == '\"' || cmd[i] == '\'')
                i = skip_quoted_text(cmd, i) + 1;
            else
                i = skip_unquoted_text(cmd, i);
        }
        else
            i++;
    }
    return tokens;
}

char **split_command(char *cmd)
{
    int total_tokens;
    char **tokens;
    int tokenIndex;
    int pos;

    tokenIndex = 0;
    pos = 0;
    total_tokens = count_tokens(cmd);
    tokens = (char **)malloc((total_tokens + 1) * sizeof(char *));
    if (!tokens)
        return NULL;
    while (tokenIndex < total_tokens)
    {
        tokens[tokenIndex] = get_token(cmd, &pos);
        if (!tokens[tokenIndex])
        {
            free_split(tokens);
            return (NULL);
        }
        tokenIndex++;
    }
    tokens[total_tokens] = NULL;
    return (tokens);
}


void handle_error_free(char **split_cmd, int exit_code)
{   
    if(split_cmd)
        free_split(split_cmd);
    exit(exit_code);
}


static int check_cmd_dot(char *cmd)
{
    int i;
    int start;
    int end;
    
    i = 0;
    while (cmd[i])
    {
        if (cmd[i] == '\"' || cmd[i] == '\'') 
        {
            start = i + 1;
            i = skip_quoted_text(cmd, i); 
            end = i; 
            if ((end - start == 1 && cmd[start] == '.') || (end - start == 0))
                return (1);
        }
        else if (cmd[i] != ' ') 
        {
            return (0); 
        }
        i++;
    }
    return (0); 
}



// static char *get_executable_path(char *first_cmd, char **split_cmd, char **env)
// {
//    	if(first_cmd && ft_strncmp(first_cmd, "..", 2) == 0)
// 	{
// 		display_error(first_cmd, "command not found");
//         handle_error_free(split_cmd, 127);

// 		// free_split(split_cmd);
// 		// exit(127);
// 	}
//     if (split_cmd[0] && (ft_strncmp(split_cmd[0], ".", 1) == 0) && ft_strlen(split_cmd[0]) == 1)
//     {
//         if (!split_cmd[1])
//         {
//             display_error(first_cmd, "filename argument required");
//             handle_error_free(split_cmd, 2);

//             // free_split(split_cmd);
//             // exit(2);
//         }
// 		char	*check_path;
// 		check_path = find_path(env);
// 		char *resolved_path = build_cmd_path(split_cmd[1], check_path);
//         if (resolved_path)
//         {
//             display_error(split_cmd[1], "cannot execute binary file");
// 			free(resolved_path);
//             handle_error_free(split_cmd, 126);

//             // free_split(split_cmd);
//             // exit(126);
//         }
// 		else
// 		{
// 			display_error(split_cmd[1], "No such file or directory");
//             handle_error_free(split_cmd, 1);

// 			// free_split(split_cmd);
// 			// exit(1);
// 		}
//     }
//     if (ft_strchr(first_cmd, '/'))
//     {
//         check_command_access(first_cmd, split_cmd);
//         return (first_cmd);
//     }
//     return get_path(first_cmd, env);
// }


static void handle_dot_cmd(char *first_cmd, char **split_cmd, char **env)
{
    char *check_path;
    char *resolved_path;

    if (split_cmd[0] && (ft_strncmp(split_cmd[0], ".", 1) == 0)
        && ft_strlen(split_cmd[0]) == 1)
    {
        if (!split_cmd[1])
        {
            display_error(first_cmd, "filename argument required");
            handle_error_free(split_cmd, 2);
        }
		check_path = find_path(env);
		resolved_path = build_cmd_path(split_cmd[1], check_path);
        if (resolved_path)
        {
            display_error(split_cmd[1], "cannot execute binary file");
			free(resolved_path);
            handle_error_free(split_cmd, 126);
        }
		else
		{
			display_error(split_cmd[1], "No such file or directory");
            handle_error_free(split_cmd, 1);
		}
    }
}

static void check_command_access(char *cmd, char **split_cmd)
{
    int fd;

    if (access(cmd, F_OK) == -1)
    {
        ft_printf_fd(2, "%s: No such file or directory\n", cmd);
        handle_error_free(split_cmd, 127);
        //display_error(cmd, "No such file or directory");
        // free_split(split_cmd);
        // exit(127);
    }
    if (access(cmd, X_OK) == -1)
    {
        ft_printf_fd(2, "%s: Permission denied\n", cmd);
        handle_error_free(split_cmd, 126);
        //display_error(cmd, "Permission denied");
        // free_split(split_cmd);
        // exit(126);
    }
    fd = open(cmd, O_DIRECTORY);
    if (fd != -1)
    {
        close(fd);
        ft_printf_fd(2, "%s: Is a directory\n", cmd);
        handle_error_free(split_cmd, 126);
        //display_error(cmd, "Is a directory");
        // free_split(split_cmd);
        // exit(126);
    }
}

static char *get_executable_path(char *first_cmd, char **split_cmd, char **env)
{
    if(first_cmd && ft_strncmp(first_cmd, "..", 2) == 0)
	{
		display_error(first_cmd, "command not found");
        handle_error_free(split_cmd, 127);
	}
    handle_dot_cmd(first_cmd, split_cmd, env);
    if (ft_strchr(first_cmd, '/'))
    {
        check_command_access(first_cmd, split_cmd);
        return (first_cmd);
    }
    return get_path(first_cmd, env);
}



void exec_cmd(char *cmd, char **env)
{
    char **split_cmd;
    char *cmd_path;

    if (!env || !cmd)
	{
        ft_printf_fd(2, "command not found\n");
		exit(127);
	}
	if (cmd[0] == '\0'|| check_white_spaces_cmd(cmd) || check_cmd_dot(cmd) == 1)
	{
		//display_error(cmd, "command not found");
        ft_printf_fd(2, "%s: command not found\n", cmd);
		exit(127);
	}
    split_cmd = split_command(cmd);
    if (!split_cmd)
        handle_error_free(split_cmd, 127);
    cmd_path = get_executable_path(split_cmd[0], split_cmd, env);
    if (!cmd_path)
        handle_error_free(split_cmd, 127);
    if (execve(cmd_path, split_cmd, env) == -1)
    {
        free_split(split_cmd);
        free(cmd_path);
        check_exit_error("execve failed");
    }
} 


