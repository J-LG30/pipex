#include "pipex_bonus.h"

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

    int i = 0;
	argument = ft_split(argv, ' ');
    // while (argument[i])
    // {
    //     printf("%s\n", argument[i]);
    //     i++;
    // }
	return (argument);
}

char	*get_command_path(char *command, char **env, int test_fd)
{
	char	*cmd_path;
	char	**new_cmd;
	char	**dirs;
	int		i;
	int		j;

    static int test_counter = 0;
	i = 0;
	j = 0;
    //printf("%s\n", command);
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	dirs = ft_split(env[i], ':');
	i = 0;
	while (dirs[i])
	{
		cmd_path = ft_strjoin(dirs[i], "/");
		cmd_path = ft_strjoin(cmd_path, command);
       // printf("%s\n", cmd_path);
		if (access(cmd_path, X_OK) == 0)
		{
			while(dirs[j++])
				free(dirs[j]);
			free(dirs);
            // write(test_fd, cmd_path, ft_strlen(cmd_path));
            // write(test_fd, ft_itoa(test_counter), 1);
            test_counter++;
			return (cmd_path);
		}
		i++;
	}
	j = 0;
	while (dirs[j++])
		free(dirs[j]);
	free(dirs);
	return (NULL);
}
