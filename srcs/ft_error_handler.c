#include "../includes/minishell.h"

/**
 * @brief Converts an integer character to a string.
 *
 * This function takes an integer character and converts it to a string. If the
 * character is 0, it prints the character to the standard output and returns 
 * NULL. Otherwise, it allocates memory for a 2-character string, assigns the 
 * character to the first position, and null-terminates the string.
 *
 * @param c The integer character to be converted.
 * @return char* Pointer to the newly allocated string, or NULL if the 
 *         character is 0.
 */
static char	*ctoa(int c)
{
	char	*str;

	if (c == 0)
	{
		ft_putchar_fd(c, 1);
		return (NULL);
	}
	str = (char *) malloc(2 * sizeof(char));
	str[0] = (char) c;
	str[1] = '\0';
	return (str);
}

/**
 * @brief Formats a single argument based on the specified format character.
 *
 * This function processes a variable argument list (`va_list`) and formats 
 * a single argument based on the provided format character. Currently, 
 * it supports the format specifier 'd' and 'i' for numbers, 's' for strings. 
 * If the argument is  `NULL`, it returns a copy of the string "(null)".
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
	else if (fmt == 'd' || fmt == 'i')
		specifier = ft_itoa(va_arg(args, int));
	else if (fmt == 'c')
		specifier = ctoa(va_arg(args, int));
	return (specifier);
}

/**
 * @brief Prints an error message to stderr with formatted arguments and
 * returns an exit status.
 *
 * This function prints a formatted error message to the standard error output 
 * (stderr) using a format string and additional args, similar to `printf`. 
 * It starts by printing a predefined program name to indicate the source of
 * the error. For each format specifier (currently only `%s` for strings), 
 * it retrieves the arg from `va_list`, formats it, and writes it to stderr
 * with a '\n'. 
 * 
 * @param str The format string specifying the output message, using `%` 
 *            as the format specifier (e.g., `%s` for strings).
 * @param ... The variable arguments matching the format specifiers in `str`.
 */
void	ft_stderror(int perror_, const char *str, ...)
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
	if (perror_ == TRUE)
		perror("");
	else
		ft_putchar_fd('\n', 2);
	va_end(args);
}

//to be printed on STDERR
int	ft_error_handler(void)
{
	printf("error to be printed on STDERR\n");
	return (-1);
}
