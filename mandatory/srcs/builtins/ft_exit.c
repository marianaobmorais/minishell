/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:31:29 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 18:02:12 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Prints "exit" message if the shell is running in an interactive mode.
 *
 * Outputs "exit" to the standard error stream if the stdin is attached 
 * to a terminal (interactive mode). Does nothing in non-interactive mode.
 */
static void	print_exit(void)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", 2);
}

/**
 * @brief Validates if a string represents a numeric value.
 *
 * Checks whether the provided string `arg` is a valid numeric argument. A valid
 * numeric argument may include an optional '+' or '-' sign followed by digits.
 * If invalid, it logs an error and sets the exit status to 2.
 *
 * @param arg The string to validate.
 * @return true if the string is numeric, false otherwise.
 */
static bool	ft_is_numeric(char *arg)
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
			return (ft_exit_status(2, TRUE, FALSE), false);
		}
		i++;
	}
	if (arg[0] == '\0' || ((arg[0] == '-' || arg[0] == '+')
			&& !ft_isdigit(arg[1])))
	{
		ft_stderror(FALSE, "exit: %s: numeric argument required", arg);
		return (false);
	}
	return (true);
}

/**
 * @brief Converts a numeric string to an unsigned char exit status.
 *
 * Converts the numeric string `arg` to a `long long` value, checking for
 * overflow and underflow. If the value exceeds the range of a valid exit status
 * (0-255), logs an error and sets the exit status to 2. If the value is valid,
 * sets the exit status accordingly and cleans up resources before exiting.
 *
 * @param arg The numeric string to convert.
 * @param args The list of command-line arguments.
 * @param sh The shell structure to clean up.
 * @return The converted exit status as an unsigned char.
 */
static unsigned char	arg_convert(char *arg, char **args, t_shell *sh)
{
	long long	num;
	bool		is_sign;

	is_sign = false;
	num = ft_atoll(arg, &is_sign);
	if (num == LLONG_MAX)
	{
		if (ft_strncmp(arg + is_sign, "9223372036854775807", 19))
		{
			ft_stderror(FALSE, "exit: %s: numeric argument required", arg);
			return (ft_exit_status(2, TRUE, FALSE));
		}
	}
	if (num == LLONG_MIN && ft_strncmp(arg, "-9223372036854775808", 20))
	{
		ft_stderror(FALSE, "exit: %s: numeric argument required", arg);
		return (ft_exit_status(2, TRUE, FALSE));
	}
	if (num == LLONG_MIN && !ft_strncmp(arg, "-9223372036854775808", 20))
	{
		ft_child_cleaner(sh, args, 1);
		ft_exit_status(0, TRUE, TRUE);
	}
	ft_exit_status((unsigned char)num, TRUE, FALSE);
	return ((unsigned char)num);
}

/**
 * @brief Handles the exit process for the shell.
 *
 * Cleans up resources and exits the shell. If no arguments are provided, exits
 * with the current exit status. If a numeric argument is provided, it validates
 * and sets the exit status accordingly. Logs an error and continues execution
 * if too many arguments are provided.
 *
 * @param argc The number of command-line arguments.
 * @param args The list of command-line arguments.
 * @param sh The shell structure to clean up.
 */
void	ft_exit(int argc, char **args, t_shell *sh)
{
	unsigned char	exit_status;

	exit_status = 0;
	print_exit();
	if (argc == 1)
	{
		ft_child_cleaner(sh, args, 1);
		ft_exit_status(exit_status, TRUE, TRUE);
	}
	if (!ft_is_numeric(args[1]))
	{
		ft_child_cleaner(sh, args, 1);
		ft_exit_status(2, TRUE, TRUE);
	}
	else if (args[2])
	{
		ft_stderror(FALSE, "exit: too many arguments");
		ft_exit_status(1, TRUE, FALSE);
	}
	else if (arg_convert(args[1], args, sh))
	{
		ft_child_cleaner(sh, args, 1);
		ft_exit_status(0, FALSE, TRUE);
	}
}
