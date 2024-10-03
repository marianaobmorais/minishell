/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:54:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/04/20 13:04:13 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	diff;
	char	*sub;
	char	*temp;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len || len == 0)
		return (ft_strdup(""));
	diff = s_len - start;
	if (len > diff)
		len = diff;
	sub = (char *)malloc((sizeof(char) * (len)) + 1);
	if (sub == NULL)
		return (NULL);
	temp = (char *)s + start;
	ft_strlcpy(sub, temp, len + 1);
	return (sub);
}
