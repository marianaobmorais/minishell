#include "../includes/minishell.h"

/**
 * @brief Formats a single argument based on the specified format character.
 *
 * This function processes a variable argument list (`va_list`) and formats 
 * a single argument based on the provided format character. Currently, 
 * it supports the format specifier 's' for strings. If the argument is 
 * `NULL`, it returns a copy of the string "(null)".
 *
 * @param args The list of arguments to format.
 * @param fmt The format specifier character (e.g., 's' for strings).
 * @return A dynamically allocated formatted string. If allocation fails,
 *         it returns "(null)" instead.
 */
static char	*ft_format(va_list args, const char fmt)
{
	char	*specifier;

	specifier = NULL;
	if (fmt == 's')
	{
		specifier = ft_strdup(va_arg(args, char *));
		if (!specifier)
			return (ft_strdup("(null)"));
	}
	return (specifier);
}

/**
 * @brief Prints an error message to stderr with formatted arguments and returns an exit status.
 *
 * This function prints a formatted error message to the standard error output 
 * (stderr) using a format string and additional arguments, similar to `printf`. 
 * It starts by printing a predefined program name (`PROG_NAME_ERROR`) to indicate 
 * the source of the error. For each format specifier (currently only `%s` for strings), 
 * it retrieves the argument from `va_list`, formats it, and writes it to stderr. 
 * The function ends by returning the specified `exit_status`.
 *
 * @param exit_status The exit code to be returned by the function.
 * @param str The format string specifying the output message, using `%` 
 *            as the format specifier (e.g., `%s` for strings).
 * @param ... The variable arguments matching the format specifiers in `str`.
 * @return The provided exit status.
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
