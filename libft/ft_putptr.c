/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpnbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:58:58 by mariaoli          #+#    #+#             */
/*   Updated: 2024/04/30 18:06:24 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_putpnbr(unsigned long n, char c)
{
	int				count;
	unsigned int	base_len;
	char			*base;

	count = 0;
	base = "0123456789abcdef";
	base_len = ft_strlen(base);
	if (n >= base_len)
	{
		count += ft_putpnbr(n / base_len, c);
		count += ft_putpnbr(n % base_len, c);
	}
	else if (c == 'x')
		count += ft_putchar(base[n]);
	return (count);
}

int	ft_putptr(unsigned long p)
{
	int	count;

	count = 0;
	if (p == 0)
		count += (ft_putstr("(nil)"));
	else
	{
		count += ft_putstr("0x");
		count += ft_putpnbr(p, 'x');
	}
	return (count);
}
