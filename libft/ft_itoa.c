/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:41:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/04/10 15:41:40 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		++len;
	while (n != 0)
	{
		n /= 10;
		++len;
	}
	return (len);
}

static void	ft_result(int n, int len, int offset, char *res)
{
	while (len > offset)
	{
		res[len - 1] = n % 10 + '0';
		n /= 10;
		--len;
	}
}

char	*ft_itoa(int n)
{
	int		offset;
	int		len;
	char	*res;

	if (n == -2147483648)
	{
		res = ft_strdup("-2147483648");
		return (res);
	}
	len = ft_intlen(n);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (res == NULL)
		return (NULL);
	offset = 0;
	if (n < 0)
	{
		res[0] = '-';
		offset = 1;
		n *= -1;
	}
	ft_result(n, len, offset, res);
	res[len] = '\0';
	return (res);
}
