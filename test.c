#include <stdio.h>


int	main(int argc, char **argv, char **env)
{
	int	i;
	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}


char	*get_command_path(char *command, char **env)
{
	int	i;

	i = 0;
	while (ft_strnstr(env[i], "PATH=", 5) == 0)
		i++;
	

}
