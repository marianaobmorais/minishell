/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:46:43 by mariaoli          #+#    #+#             */
/*   Updated: 2024/04/16 20:00:23 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*temp;
	char	chr;

	temp = (char *)s;
	chr = (char)c;
	while (*temp != '\0')
		++temp;
	while (*temp != chr && temp != s)
		--temp;
	if (*temp == chr)
		return (temp);
	return (NULL);
}
