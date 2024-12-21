/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:02:05 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/21 12:33:49 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Checks if a given string is a valid `-n` flag.
 *
 * This function checks if the input string is in the form of the `-n` flag, 
 * which may consist of multiple consecutive `n` characters (e.g., `-n`,
 * `-nnn`). It returns `true` if the string is a valid flag, and `false`
 * otherwise.
 *
 * @param s The string to check.
 * @return `true` if the string is a valid `-n` flag, `false` otherwise.
 */
static bool	ft_arg_is_flag(char *s)
{
	int	i;

	if (!(s[0] == '-' && s[1] == 'n'))
		return (false);
	else
	{
		i = 2;
		while (s[i] == 'n')
			i++;
		if (s[i] == '\0')
			return (true);
		return (false);
	}
}

/**
 * @brief Mimics the behavior of the `echo` command with optional flags.
 *
 * This function implements the `echo` command, printing the arguments passed 
 * to it. If the `-n` flag is present, it suppresses the newline at the end of
 * the output. If no arguments are passed, it simply prints a newline. The
 * function handles multiple consecutive `-n` flags and correctly formats the
 * output.
 *
 * @param args The array of arguments passed to the `echo` command.
 */
int	ft_echo(char **args)
{
	bool	flag;
	int		i;

	if (!args[1])
	{
		printf("\n");
		return (ft_exit_status(0, TRUE, FALSE));
	}
	flag = false;
	i = 1;
	while (args[i] && ft_arg_is_flag(args[i]))
	{
		flag = true;
		i++;
	}
	while (args && args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	return (ft_exit_status(0, TRUE, FALSE));
}

/* int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	ft_echo("-nnnnnnnnnnnnnnnnnnnn -nn -no -n oi   'oi'oi'' \"tchau\"");
	return (0);
} */

/* int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char	**s;

	//s = ft_split_argv("-nnnnnnnnnnnnnnnnnnnn -nn -no -n oi \"$USER\"  \
		'oi'oi'' \"tchau\" espaco ", ' ', envp);
	s = ft_split_argv("$USER tchau", ' ', envp);
	ft_echo(s);
	ft_free_vector(s);
	return (0);
} */
