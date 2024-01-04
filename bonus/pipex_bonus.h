/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:18:19 by jle-goff          #+#    #+#             */
/*   Updated: 2023/12/20 17:11:48 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include "../libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>

# ifndef MAX_ARG
#  define MAX_ARG 500
# endif

typedef struct t_file
{
	int	filein;
	int	fileout;
	int	argc;
}	t_file;

typedef struct t_pipex
{
	int		fd[2];
	pid_t	child_pids[MAX_ARG];
}	t_pipex;

void	error_quit(int type, int fd[2], int fileout);
char	**create_arg(char *argv);
char	*get_command_path(char *command, char **env, int test_fd);
void	free_arg(char **arg);

#endif