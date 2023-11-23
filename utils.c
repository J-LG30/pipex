/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:28:15 by jle-goff          #+#    #+#             */
/*   Updated: 2023/11/23 17:33:17 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
