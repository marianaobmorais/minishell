/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:45:44 by mariaoli          #+#    #+#             */
/*   Updated: 2024/04/10 15:45:45 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	const char	*srcptr;
	size_t		srclen;
	size_t		i;

	srcptr = src;
	srclen = 0;
	while (*srcptr != '\0')
	{
		++srcptr;
		++srclen;
	}
	i = dstsize;
	if (dstsize > 0)
	{
		while ((i - 1 > 0) && (*src != '\0'))
		{
			*dst = *src;
			++dst;
			++src;
			--i;
		}
		*dst = '\0';
	}
	return (srclen);
}
