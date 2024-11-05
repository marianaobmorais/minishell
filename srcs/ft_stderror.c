#include "../includes/minishell.h"

static char	*str_check(char *specifier)
{
	if (!specifier)
		return (ft_strdup("(null)"));
	else
		return (ft_strdup(specifier));
}

static char	*ft_format(va_list args, const char fmt)
{
	char	*specifier;

	specifier = NULL;
	if (fmt == 's')
		specifier = str_check(va_arg(args, char *));
	return (specifier);
}

int	ft_stderror(int error, const char *str, ...)
{
	va_list	args;
	char	*fmt_specifier;

	va_start(args, str);
	ft_putstr_fd("bashinho: ", 2);
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			fmt_specifier = ft_format(args, *str);
			ft_putstr_fd(fmt_specifier, 2);
			free(fmt_specifier);
		}
		else
			ft_putchar_fd(*str, 2);
		str++;
	}
	va_end(args);
	return (error);
}
