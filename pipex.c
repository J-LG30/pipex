#include "pipex.h"

void	write_to_pipe(int fd, char *cmd_path, char **argument, char *filename, char **env)
{
	int	file1;

	file1 = open(filename, O_RDONLY);
	dup2(file1, 0);
	dup2(fd, 1);
	execve(cmd_path, argument, env);
}

//The second command executes whatever is written at
//the fd[0] of the pipe (the reading fd)
//eg: it would execute ls on fd[0]
void	read_from_pipe(int fd, char *filename, char *cmd_path, char **argument, char **env)
{
	int		file2;

	file2 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file2 == -1)
		printf("error opening file\n");
	dup2(file2, 1);
	dup2(fd, 0);
	execve(cmd_path, argument, env);
	printf("Parent execve error\n");
}

char	**create_arg(char *argv, char *filename)
{
	char	**argument;
	char	**new_array;
	int		array_size;
	int		i;

	
	array_size = 0;
	argument = ft_split(argv, ' ');
	// while (argument[array_size])
	// 	array_size++;
	// new_array = malloc(sizeof(char*) * (array_size + 1));
	// i = 0;
	// array_size = 0;
	
	// while (argument[array_size])
	// {
	// 	new_array[i] = ft_strdup(argument[array_size]);
	// 	i++;
	// 	array_size++;
	// }
	
	// if (!filename)
	// 	new_array[i] = NULL;
	// else
	// 	new_array[i] = ft_strdup(filename);
	// new_array[i + 1] = NULL;
	// i = 0;
	// free(argument);
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
	char	*slash_new_cmd;
	char	*cmd_path;
	char	**new_cmd;
	char	**dirs;
	int		i;

	i = 0;
	new_cmd = ft_split(command, ' ');
	slash_new_cmd = ft_strdup("/");
	ft_strlcat(slash_new_cmd, new_cmd[0], 100);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			dirs = ft_split(env[i], ':');
			i = 0;
			while (dirs[i])
			{
				cmd_path = ft_strjoin(dirs[i], slash_new_cmd);
				if (access(cmd_path, X_OK) == 0)
				{
					return (cmd_path);
				}
				i++;
			}
			return (NULL);
		}
		else
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
	{
		printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
		exit (1);
	}
	//create pipe and handle error
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit (1);
	}
	//Fork to create parent and child process
	//handle fork error
	if ((pid = fork()) == -1)
	{
		perror("fork");
		exit (1);
	}
	//if child process cmd2
	//child process reads from pipe and uses that to execute command 2
	if (pid == 0)
	{
		close(fd[0]);
		cmd_path = get_command_path(argv[2], env);
		if (!cmd_path)
		{
			printf("Couldnt retrieve command path\n");
			exit (1);
		}
		write_to_pipe(fd[1], cmd_path, create_arg(argv[2], argv[1]), argv[1], env);
		exit(0);	
	}
	//parent writes into pipe the ouput of command 1
	else
	{
		//printf("%s\n", argv[4]);
		waitpid(-1, NULL, 0);
		close(fd[1]);
		cmd_path = get_command_path(argv[3], env);
		if (!cmd_path)
		{
			printf("Couldnt retrieve command path\n");
			exit (1);
		}
		read_from_pipe(fd[0], argv[4], cmd_path, create_arg(argv[3], NULL), env);
		exit(0);	
	}
}