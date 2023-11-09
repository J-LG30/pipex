# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int	ft_strncmp( const char *str1, const char *str2, size_t num)
{
	size_t	i;

	i = 0;
	if (num == 0)
		return (0);
	while (str1[i] == str2[i] && (str1[i] != '\0'
			|| str2[i] != '\0') && i < num - 1)
	{
		i++;
	}
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}


int main(int argc, char **argv, char **env)
{
    int i = 0;

    while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
		{
			printf("%s\n", env[i]);
            exit (1);
		}
		i++;
	}
    // int i = 0;
    // while (env[i])
    // {
    //     printf("%s\n", env[i]);
    //     i++;
    // }
}