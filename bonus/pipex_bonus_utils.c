/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 11:50:14 by jle-goff          #+#    #+#             */
/*   Updated: 2023/12/18 13:16:02 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_arg(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		free(arg[i++]);
	free(arg);
}

void	error_quit(int type)
{
	if (type == 1)
		write(1, "Usage: ./pipex file1 cmd1 cmd2 ... file2\n", 41);
	if (type == 2)
		perror("pipe");
	if (type == 3)
		perror("fork");
	if (type == 4)
		write(1, "Could not retrieve command path\n", 32);
	exit (1);
}

char	**create_arg(char *argv)
{
	char	**argument;

	argument = ft_split(argv, ' ');
	return (argument);
}

char	*get_command_path(char *command, char **env, int test_fd)
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
	while (dirs[i])
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
