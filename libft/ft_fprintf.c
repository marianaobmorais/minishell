/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:12:27 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/09 15:46:38 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_parse_fmt_fd(va_list ap, const char fmt, int fd)
{
	int	count;

	count = 0;
	if (fmt == 'c')
		count += ft_putchar_fd(va_arg(ap, int), fd);
	if (fmt == 's')
		count += ft_putstr_fd(va_arg(ap, char *), fd);
	if (fmt == 'd' || fmt == 'i')
		count += ft_putnbr_fd(va_arg(ap, int), fd);
	if (fmt == 'u')
		count += ft_putunbr_fd(va_arg(ap, unsigned int), fd);
	if (fmt == 'x' || fmt == 'X')
		count += ft_putxnbr_fd(va_arg(ap, unsigned int), fmt, fd);
	if (fmt == 'p')
		count += ft_putptr(va_arg(ap, unsigned long));
	if (fmt == '%')
		count += ft_putchar_fd('%', fd);
	return (count);
}

int	ft_fprintf(int fd, const char *format, ...)
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
			res += ft_parse_fmt_fd(ap, *fmt, fd);
		}
		else
			res += ft_putchar_fd(*fmt, fd);
		fmt++;
	}
	va_end(ap);
	return (res);
}
