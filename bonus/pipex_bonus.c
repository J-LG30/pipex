/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:17:17 by jle-goff          #+#    #+#             */
/*   Updated: 2023/12/08 16:17:30 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"


//free if execve fails
void	write_to_pipe(int fd, char *cmd_path, char **argument, char **env)
{

	dup2(fd, 1);
	close(fd);
	execve(cmd_path, argument, env);
	free(cmd_path);
}

int	main(int argc, char **argv, char **env)
{
	int		fd[2];
	pid_t	pid;
	char	*cmd_path;
	int		i;
	int		fileout;
	int		filein;
	char	**arg;

	if (argc < 5)
		error_quit(1);
	i = 2;
	fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	filein = open(argv[1], O_RDONLY);
	dup2(filein, 0);
	while (i < argc - 2)
	{
		if (pipe(fd) == -1)
			error_quit(2);
		pid = fork();
		if (pid == -1)
			error_quit(3);
		if (pid == 0)
		{
			arg = create_arg(argv[i]);
			close(fd[0]);
			cmd_path = get_command_path(arg[0], env, fd[1]);
			if (!cmd_path)
				error_quit(4);
			close(fileout);
			close(filein);
			write_to_pipe(fd[1], cmd_path, arg, env);
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], 0);
			waitpid(pid, NULL, 0);
		}
		i++;
	}
	dup2(fileout, 1);
	//close(fileout);
	close(filein);
	arg = create_arg(argv[i]);
	cmd_path = get_command_path(arg[0], env, fd[1]);
	//printf("%s\n", cmd_path);
	execve(cmd_path, arg, env);
	exit(0);
}
