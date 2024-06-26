/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:19:08 by jle-goff          #+#    #+#             */
/*   Updated: 2024/01/04 17:36:52 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	write_to_pipe(int fd, char *cmd_path, char **argument, char *filename)
{
	int	file1;

	file1 = open(filename, O_RDONLY);
	if (file1 == -1)
		error_quit(5, (int [2]){fd, 0}, 0);
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
		error_quit(5, (int [2]){fd, 0}, 0);
	dup2(file2, 1);
	dup2(fd, 0);
	close(file2);
	close(fd);
	execve(cmd_path, argument, NULL);
}

void	first_cmd(int fd[2], char **env, char **argv)
{
	char	**arg;
	char	*cmd_path;

	close(fd[0]);
	arg = create_arg(argv[2]);
	cmd_path = get_command_path(arg[0], env);
	if (!cmd_path)
	{
		free_arg(arg);
		error_quit(4, (int [2]){fd[1], 0}, 0);
	}
	write_to_pipe(fd[1], cmd_path, arg, argv[1]);
}

void	second_cmd(int fd[2], char **env, char **argv)
{
	char	**arg;
	char	*cmd_path;

	close(fd[1]);
	arg = create_arg(argv[3]);
	cmd_path = get_command_path(arg[0], env);
	if (!cmd_path)
	{
		free_arg(arg);
		error_quit(4, (int [2]){fd[0], 0}, 0);
	}
	read_from_pipe(fd[0], argv[4], cmd_path, arg);
}

int	main(int argc, char **argv, char **env)
{
	int		fd[2];
	char	*cmd_path;
	char	**arg;
	pid_t	pid_1;
	pid_t	pid_2;

	if (argc != 5)
		error_quit(1, 0, 0);
	if (pipe(fd) == -1)
		error_quit(2, 0, 0);
	pid_1 = fork();
	if (pid_1 == -1)
		error_quit(3, fd, 0);
	if (pid_1 == 0)
		first_cmd(fd, env, argv);
	pid_2 = fork();
	if (pid_2 == -1)
		error_quit(3, fd, 0);
	if (pid_2 == 0)
		second_cmd(fd, env, argv);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_1, NULL, 0);
	waitpid(pid_2, NULL, 0);
}

// int	main(int argc, char **argv, char **env)
// {
// 	int		fd[2];
// 	pid_t	pid;
// 	char	*cmd_path;
// 	char	**arg;
// 	pid_t	pid_1;
// 	pid_t	pid;
// 	char	*cmd_path;
// 	char	**arg;
// 	pid_t	pid_1;
// 	pid_t	pid_2;
// 	pid_t	pid_2;

// 	if (argc != 5)
// 		error_quit(1);
// 	if (pipe(fd) == -1)
// 		error_quit(2);
// 	pid = fork();
// 	if (pid == -1)
// 		error_quit(3);
// 	if (pid == 0)
// 	{
// 		close(fd[0]);
// 		arg = create_arg(argv[2]);
// 		cmd_path = get_command_path(arg[0], env);
// 		if (!cmd_path)
// 			error_quit(4);
// 		write_to_pipe(fd[1], cmd_path, arg, argv[1]);
// 		free(cmd_path);
// 	}
// 	else
// 	{
// 		close(fd[1]);
// 		arg = create_arg(argv[3]);
// 		cmd_path = get_command_path(arg[0], env);
// 		if (!cmd_path)
// 			error_quit(4);
// 		read_from_pipe(fd[0], argv[4], cmd_path, arg);
// 		free(cmd_path);
// 	}
// }
