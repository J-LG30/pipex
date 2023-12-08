#include <stdio.h>
#include "pipex_bonus.h"

int	ft_strncmp(const char *str1, const char *str2, size_t num)
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

char	*cp_word(char const *s, int start, int end)
{
	char	*str;
	int		i;

	i = 0;
	str = malloc(sizeof(char) * (end - start + 1));
	if (str == NULL)
		return (0);
	while (start < end)
	{
		str[i] = s[start];
		i++;
		start++;
	}
	str[i] = '\0';
	return (str);
}

int	str_size(char const *s, int index, char c)
{
	int	size;

	size = 0;
	while (s[index] != c && s[index] != '\0')
	{
		index++;
		size++;
	}
	return (size);
}

int	count_words(char const *s, char c)
{
	int	numstr;
	int	i;

	i = 0;
	numstr = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			numstr++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		i++;
	}
	return (numstr);
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		num;
	int		end;
	int		words;

	i = 0;
	num = 0;
	str = malloc(sizeof(char *) * (count_words(s, c) + 1));
	words = count_words(s, c);
	if (str == NULL)
		return (0);
	while (s[i] && num < words)
	{
		while (s[i] == c)
			i++;
		end = str_size(s, i, c);
		str[num] = cp_word(s, i, end + i);
		if (str[num] == NULL)
			return (0);
		i += end;
		num++;
	}
	str[num] = 0;
	return (str);
}

int	main(int argc, char **argv, char **env)
{
	char **dirs;
	int	i = 0;
	int	j = 0;

	while(env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	printf("%s\n", env[i]);
	dirs = ft_split(env[i], ':');

}
