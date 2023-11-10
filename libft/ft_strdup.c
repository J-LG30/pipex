/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-goff <jle-goff@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 16:35:07 by jle-goff          #+#    #+#             */
/*   Updated: 2023/11/10 14:59:20 by jle-goff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char *src)
{
	char	*dst;

	dst = malloc(ft_strlen(src) +1);
	if (dst == NULL)
		return (NULL);
	else
		ft_memcpy(dst, src, ft_strlen(src) + 1);
	return (dst);
}
/*
int main(void)
{
	char * a = "";
	printf("%s", ft_strdup(a));
}
*/
