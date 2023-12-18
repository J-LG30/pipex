/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:18:19 by jle-goff          #+#    #+#             */
/*   Updated: 2023/12/18 12:59:29 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef struct t_file
{
	int	filein;
	int	fileout;
}	t_file;

void	error_quit(int type);
char	**create_arg(char *argv);
char	*get_command_path(char *command, char **env, int test_fd);
void	free_arg(char **arg);