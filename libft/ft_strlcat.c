/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:45:33 by mariaoli          #+#    #+#             */
/*   Updated: 2024/04/10 15:45:34 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dstlen;
	size_t	srclen;

	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	if (dstlen >= size)
		return (size + srclen);
	while ((*dst != '\0') && (size - 1 > 0))
	{
		++dst;
		--size;
	}
	while ((*src != '\0') && (size - 1 > 0))
	{
		*dst = *src;
		++dst;
		++src;
		--size;
	}
	*dst = '\0';
	return (dstlen + srclen);
}
