#include "pipex_bonus.h"

void	error_quit(int type)
{
	if (type == 1)
		write(1, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37);
	if (type == 2)
		perror("pipe");
	if (type == 3)
		perror("fork");
	if (type == 4)
		write(1, "Could not retrieve command path\n", 32);
	exit (1);
}

//free if execve fails
void	write_to_pipe(int fd, char *cmd_path, char **argument, char *filename)
{
	int	file1;

	dup2(fd, 1);
	execve(cmd_path, argument, NULL);
	free(cmd_path);
}

void	read_from_pipe(int fd, char *filename, char *cmd_path, char **argument)
{
	int		file2;

	dup2(fd, 0);
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
			dirs = ft_split(env[i], ':');
			i = 0;
			while (dirs[i])
			{
				new_cmd = ft_split(command, ' ');
				new_cmd[0] = ft_strjoin("/", new_cmd[0]);
				cmd_path = ft_strjoin(dirs[i], new_cmd[0]);
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
	int		i;
	int		fileout;
	int		filein;

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
			close(fd[0]);
			cmd_path = get_command_path(argv[i], env);
			if (!cmd_path)
				error_quit(4);
			write_to_pipe(fd[1], cmd_path, create_arg(argv[i]), argv[1]);
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
	cmd_path = get_command_path(argv[i], env);
	execve(cmd_path, create_arg(argv[i]), env);
	exit(0);
}
