/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 16:15:31 by jle-goff          #+#    #+#             */
/*   Updated: 2023/12/20 17:50:17 by jle-goff         ###   ########.fr       */
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
	execve(cmd_path, arg, env);
}

// void	end_cmd(t_file files, t_pipex pipex, char **argv, char **env)
// {
// 	int	last_cmd;

// 	last_cmd = files.argc - 4;
// 	pipex.child_pids[last_cmd] = fork();
// 	if (pipex.child_pids[last_cmd] == -1)
// 		error_quit(3, pipex.fd, files.fileout);
// 	if (pipex.child_pids[last_cmd] == 0)
// 	{
// 		close(pipex.fd[0]);
// 		write_to_pipe(files.fileout, env, argv[files.argc - 2]);
// 	}
// }

void	pipe_loop(t_file files, t_pipex *pipex, char **argv, char **env)
{
	int	i;
	int	j;
	int	last_cmd;

	i = 2;
	while (i < files.argc - 2)
	{
		if (pipe(pipex->fd) == -1)
			error_quit(2, 0, files.fileout);
		pipex->child_pids[i - 2] = fork();
		if (pipex->child_pids[i - 2] == -1)
			error_quit(3, pipex->fd, files.fileout);
		if (pipex->child_pids[i - 2] == 0)
		{
			close(pipex->fd[0]);
			close(files.fileout);
			write_to_pipe(pipex->fd[1], env, argv[i]);
		}
		close(pipex->fd[1]);
		dup2(pipex->fd[0], 0);
		close(pipex->fd[0]);
		i++;
	}
}

void	open_files(t_file *files, char **argv, int argc)
{
	files->fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	files->filein = open(argv[1], O_RDONLY);
	files->argc = argc;
	dup2(files->filein, 0);
	close(files->filein);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	pipex;
	t_file	files;
	int		j;
	int		i;
	int		last_cmd;

	if (argc < 5)
		error_quit(1, 0, 0);
	open_files(&files, argv, argc);
	i = 2;
	j = -1;
	pipe_loop(files, &pipex, argv, env);
	last_cmd = files.argc - 4;
	pipex.child_pids[last_cmd] = fork();
	if (pipex.child_pids[last_cmd] == -1)
		error_quit(3, pipex.fd, files.fileout);
	if (pipex.child_pids[last_cmd] == 0)
	{
		close(pipex.fd[0]);
		write_to_pipe(files.fileout, env, argv[files.argc - 2]);
	}
	while (++j < files.argc - 3)
		waitpid(pipex.child_pids[j], NULL, 0);
	close(files.fileout);
}

// int	main(int argc, char **argv, char **env)
// {
// 	t_pipex	pipex;
// 	t_file	files;
// 	int		j;
// 	int		i;
// 	int		last_cmd;

// 	// open_files(files, argv, argc);
// 	// printf("%i", files.argc);
// 	// exit (1);
// 	if (argc < 5)
// 		error_quit(1, 0, 0);
// 	files.fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
// 	files.filein = open(argv[1], O_RDONLY);
// 	files.argc = argc;
// 	dup2(files.filein, 0);
// 	close(files.filein);

// 	i = 2;
// 	// printf("%i", files.argc);
// 	// exit(1);
// 	while (i < files.argc - 2)
// 	{
// 		if (pipe(pipex.fd) == -1)
// 			error_quit(2, 0, files.fileout);
// 		pipex.child_pids[i - 2] = fork();
// 		if (pipex.child_pids[i - 2] == -1)
// 			error_quit(3, pipex.fd, files.fileout);
// 		if (pipex.child_pids[i - 2] == 0)
// 		{
// 			close(pipex.fd[0]);
// 			close(files.fileout);
// 			write_to_pipe(pipex.fd[1], env, argv[i]);
// 		}
// 		close(pipex.fd[1]);
// 		dup2(pipex.fd[0], 0);
// 		close(pipex.fd[0]);
// 		i++;
// 		//printf("%i\n", i);
// 	}	
// 	last_cmd = argc - 4;
// 	//pipe_loop(files, pipex, argv, env);
// 	pipex.child_pids[last_cmd] = fork();
// 	if (pipex.child_pids[last_cmd] == -1)
// 		error_quit(3, pipex.fd, files.fileout);
// 	if (pipex.child_pids[last_cmd] == 0)
// 	{
// 		close(pipex.fd[0]);
// 		write_to_pipe(files.fileout, env, argv[argc - 2]);
// 	}
// 	j = -1;
// 	while (++j < argc - 3)
// 		waitpid(pipex.child_pids[j], NULL, 0);
// 	close(files.fileout);
// }