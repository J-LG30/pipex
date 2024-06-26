/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:17:17 by jle-goff          #+#    #+#             */
/*   Updated: 2023/12/22 19:14:03 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	write_to_pipe(int fd, char **env, char *argv)
{
	char	**arg;
	char	*cmd_path;

	arg = create_arg(argv);
	cmd_path = get_command_path(arg[0], env, fd);
	if (!cmd_path)
	{
		close(fd);
		free_arg(arg);
		error_quit(4, 0, 0);
	}
	dup2(fd, 1);
	close(fd);
	perror(cmd_path);
	execve(cmd_path, arg, env);
	free(cmd_path);
}

void	pipe_loop(int fd[2], t_file files, char *argv, char **env)
{
	pid_t	pid;

	if (pipe(fd) == -1)
		error_quit(2, 0, files.fileout);
	pid = fork();
	if (pid == -1)
		error_quit(3, fd, files.fileout);
	if (pid == 0)
	{
		close(fd[0]);
		close(files.fileout);
		close(files.filein);
		write_to_pipe(fd[1], env, argv);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		waitpid(pid, NULL, 0);
	}
}

int	main(int argc, char **argv, char **env)
{
	int		fd[2];
	pid_t	pid;
	int		i;
	t_file	files;

	if (argc < 5)
		error_quit(1, 0, 0);
	i = 2;
	files.fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (files.fileout == -1)
		error_quit(5, 0, 0);
	files.filein = open(argv[1], O_RDONLY);
	dup2(files.filein, 0);
	while (i < argc - 2)
	{
		pipe_loop(fd, files, argv[i], env);
		i++;
	}
	close(files.filein);
	write_to_pipe(files.fileout, env, argv[i]);
}
