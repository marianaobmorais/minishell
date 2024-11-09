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

/**
 * @brief Print in fd stderr a string.
 *
 * This function print a string with a another string , 
 * which may consist of multiple consecutive `n` characters (e.g., `-n`, `-nnn`).
 * It returns `true` if the string is a valid flag, and `false` otherwise.
 *
 * @param s The string to check.
 * @return `true` if the string is a valid `-n` flag, `false` otherwise.
 */
int	ft_stderror(int exit_status, const char *str, ...)
{
	va_list	args;
	char	*fmt_specifier;

	va_start(args, str);
	ft_putstr_fd(PROG_NAME_ERROR, 2);
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
	return (exit_status);
}

//to be printed on STDERR
int	ft_error_handler(void)
{
	printf("error to be printed on STDERR\n");
	return (-1);
}
