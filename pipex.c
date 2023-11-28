/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:19:08 by jle-goff          #+#    #+#             */
/*   Updated: 2023/11/23 18:17:56 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	write_to_pipe(int fd, char *cmd_path, char **argument, char *filename)
{
	int	file1;

	file1 = open(filename, O_RDONLY);
	if (file1 == -1)
		printf("error opening file\n");
	dup2(file1, 0);
	dup2(fd, 1);
	close(file1);
	close(fd);
	execve(cmd_path, argument, NULL);
}

//The second command executes whatever is written at
//the fd[0] of the pipe (the reading fd)
//eg: it would execute ls on fd[0]
void	read_from_pipe(int fd, char *filename, char *cmd_path, char **argument)
{
	int		file2;

	file2 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file2 == -1)
		printf("error opening file\n");
	dup2(file2, 1);
	dup2(fd, 0);
	close(file2);
	close(fd);
	execve(cmd_path, argument, NULL);
	printf("Parent execve error\n");
}

char	**create_arg(char *argv)
{
	char	**argument;

	argument = ft_split(argv, ' ');
	return (argument);
}

//find i value for string equaling all the directories with exectuables
//split between colons
//go through all the strings provided by split
//check if you can execute command in that directory
//if not go on
//if you can you found the path for the command
char	*get_command_path(char *command, char **env)
{
	char	*cmd_path;
	char	**new_cmd;
	char	**dirs;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			printf("%s\n", env[i]);
			// exit(1);
			dirs = ft_split(env[i], ':');
			i = 0;
			while (dirs[i])
			{
				new_cmd = ft_split(command, ' ');
				printf("%s\n", dirs[i]);

				new_cmd[0] = ft_strjoin("/", new_cmd[0]);
				cmd_path = ft_strjoin(dirs[i], new_cmd[0]);
				printf("%s\n", cmd_path);
				exit(1);
				if (access(cmd_path, X_OK) == 0)
				{
					i = -1;
					while (dirs[++i])
						free(dirs[i]);
					free(dirs);
					i = -1;
					while (new_cmd[++i])
						free(new_cmd[i]);
					free(new_cmd);
					return (cmd_path);
				}
				i++;
			}
		}
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv, char **env)
{
	int		fd[2];
	pid_t	pid;
	char	*cmd_path;

	if (argc != 5)
		error_quit(1);
	if (pipe(fd) == -1)
		error_quit(2);
	pid = fork();
	if (pid == -1)
		error_quit(3);
	if (pid == 0)
	{
		close(fd[0]);
		cmd_path = get_command_path(argv[2], env);
		if (!cmd_path)
			error_quit(4);
		write_to_pipe(fd[1], cmd_path, create_arg(argv[2]), argv[1]);
		free(cmd_path);
		exit(0);
	}
	else
	{
		waitpid(-1, NULL, 0);
		close(fd[1]);
		cmd_path = get_command_path(argv[3], env);
		if (!cmd_path)
			error_quit(4);
		read_from_pipe(fd[0], argv[4], cmd_path, create_arg(argv[3]));
		free(cmd_path);
		exit(0);
	}
}
