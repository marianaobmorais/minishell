/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:43:39 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/09 14:21:04 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	int	count;

	count = 0;
	if (n == -2147483648)
	{
		write (fd, "-2147483648", 11);
		return (11);
	}
	else
	{
		if (n < 0)
		{
			count += ft_putchar_fd('-', fd);
			n *= -1;
		}
		if (n < 10)
			count += ft_putchar_fd(n + '0', fd);
		if (n > 9)
		{
			count += ft_putnbr_fd(n / 10, fd);
			count += ft_putnbr_fd(n % 10, fd);
		}
	}
	return (count);
}
