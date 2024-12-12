#include "../../includes/minishell.h"

long long	ft_check_flow(int digit, long long sign, long long res)
{
	if (res > LLONG_MAX / 10
		|| (res == LLONG_MAX / 10 && digit > LLONG_MAX % 10))
	{
		if (sign == 1)
			return (LLONG_MAX);
		else
			return (LLONG_MIN);
	}
	return (res);
		
}

long long	ft_atoll(const char *nptr)
{
	int			i;
	long long	sign;
	long long	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((nptr[i] == 32) || (nptr[i] > 8 && nptr[i] < 14))
		++i;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		++i;
	}
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		res = ft_check_flow(nptr[i] - '0', sign, res);
		if (res > LLONG_MAX || res < LLONG_MIN)
			return (res);
		res = res * 10 + nptr[i] - '0';
		++i;
	}
	return (res * sign);
}

/**
 * @brief Prints "exit" message if the shell is running in an interactive mode.
 *
 * Outputs "exit" to the standard error stream if the stdin is attached 
 * to a terminal (interactive mode). Does nothing in non-interactive mode.
 */
static void	print_exit(void)
{
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
}

/**
 * @brief Checks if a string argument is not a valid numeric value.
 *
 * Determines whether the given string is not numeric. Accepts optional '+' or 
 * '-' prefixes followed by digits. Logs an error if the argument is invalid.
 *
 * @param arg The string to validate.
 * @return TRUE if the string is not numeric, FALSE otherwise.
 */
static int	isnumeric(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_stderror(FALSE, "exit: %s: numeric argument required", arg);
			return (ft_exit_status(2, TRUE, FALSE));
		}
		i++;
	}
	if (arg[0] == '\0' || ((arg[0] == '-' || arg[0] == '+')
			&& !ft_isdigit(arg[1])))
	{
		ft_stderror(FALSE, "exit: %s: numeric argument required", arg);
		return (TRUE);
	}
	return (FALSE);
}

/**
 * @brief Converts a string argument to a numeric value with validation.
 *
 * Validates and converts a string argument to a long integer. If the argument 
 * is not numeric or exceeds the range of a 32-bit integer, it returns an exit 
 * status of 2 and logs an error. Handles optional '+' or '-' prefixes.
 *
 * @param arg The string argument to convert.
 * @return The converted numeric value on success or triggers an error with 
 *         an exit status of 2 if validation fails.
 */
static unsigned char	arg_convert(char *arg)
{
	long long	num;

	num = ft_atoll(arg);
	if ((num > LLONG_MAX || num < LLONG_MIN) 
		|| (num == LLONG_MAX && ft_strcmp(arg, "9223372036854775807"))
		|| (num == LLONG_MIN && ft_strcmp(arg, "-9223372036854775808"))) //double check this at school
	{
		ft_stderror(FALSE, "exit: %s: numeric argument required", arg);
		return (ft_exit_status(2, TRUE, FALSE));
	}
	ft_exit_status((unsigned char)num, TRUE, FALSE);
	return ((unsigned char)num);
}

/**
 * @brief Handles the exit command
 *
 * Processes arguments for the `exit` command, validating them and determining 
 * the appropriate exit status. Handles invalid numeric arguments, excess 
 * arguments, or no arguments. Exits with the specified status if valid.
 *
 * @param argc The number of arguments passed to the exit command.
 * @param args The array of arguments, where args[0] is the command name.
 */
void	ft_exit(int argc, char **args, t_shell *sh)
{
	unsigned char	exit_status;
	int				i;

	i = 1;
	exit_status = 0;
	print_exit();
	if (argc == 1)
	{
		ft_child_cleaner(sh, args, 1);
		ft_exit_status(exit_status, TRUE, TRUE);
	}
	if (isnumeric(args[1]))
	{
		ft_child_cleaner(sh, args, 1);
		ft_exit_status(2, TRUE, TRUE);
	}
	else if (args[2])
	{
		ft_stderror(FALSE, "exit: too many arguments");
		ft_exit_status(1, TRUE, FALSE);
	}
	else
	{
		if (arg_convert(args[i]) && !args[i + 1])
		{
			ft_child_cleaner(sh, args, 1);
			ft_exit_status(0, FALSE, TRUE);
		}
	}
}
