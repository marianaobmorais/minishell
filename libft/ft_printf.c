/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:19:25 by mariaoli          #+#    #+#             */
/*   Updated: 2024/05/02 19:42:52 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_parse_fmt(va_list ap, const char fmt)
{
	int	count;

	count = 0;
	if (fmt == 'c')
		count += ft_putchar(va_arg(ap, int));
	if (fmt == 's')
		count += ft_putstr(va_arg(ap, char *));
	if (fmt == 'd' || fmt == 'i')
		count += ft_putnbr(va_arg(ap, int));
	if (fmt == 'u')
		count += ft_putunbr(va_arg(ap, unsigned int));
	if (fmt == 'x' || fmt == 'X')
		count += ft_putxnbr(va_arg(ap, unsigned int), fmt);
	if (fmt == 'p')
		count += ft_putptr(va_arg(ap, unsigned long));
	if (fmt == '%')
		count += ft_putchar('%');
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list		ap;
	const char	*fmt;
	int			res;

	va_start(ap, format);
	fmt = format;
	res = 0;
	while (*fmt != '\0')
	{
		if (*fmt == '%' && *(fmt + 1) != '\0')
		{
			fmt++;
			res += ft_parse_fmt(ap, *fmt);
		}
		else
			res += ft_putchar(*fmt);
		fmt++;
	}
	va_end(ap);
	return (res);
}
