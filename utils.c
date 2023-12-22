/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:28:15 by jle-goff          #+#    #+#             */
/*   Updated: 2023/12/22 19:05:43 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_quit(int type, int fd[2], int file)
{
	if (type == 1)
		write(1, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37);
	if (type == 2)
		perror("Pipe failed");
	if (type == 3)
	{
		close(fd[0]);
		close(fd[1]);
		perror("Fork failed");
	}
	if (type == 4)
		write(1, "Could not retrieve command path\n", 32);
	if (type == 5)
	{
		close(fd[0]);
		perror("Error opening file");
	}
	exit (1);
}

char	**create_arg(char *argv)
{
	char	**argument;

	argument = ft_split(argv, ' ');
	return (argument);
}

void	free_arg(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		free(arg[i++]);
	free(arg);
}

char	*get_command_path(char *command, char **env)
{
	char	*incomplete_path;
	char	*cmd_path;
	char	**new_cmd;
	char	**dirs;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	dirs = ft_split(env[i], ':');
	i = 0;
	while (dirs && dirs[i])
	{
		incomplete_path = ft_strjoin(dirs[i++], "/");
		cmd_path = ft_strjoin(incomplete_path, command);
		free(incomplete_path);
		if (access(cmd_path, X_OK) == 0)
		{
			free_arg(dirs);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free_arg(dirs);
	return (NULL);
}
