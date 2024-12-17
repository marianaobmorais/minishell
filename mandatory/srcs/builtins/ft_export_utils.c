/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:31:35 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:31:36 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Sorts an array of strings in ascending order using bubble sort.
 *
 * This function sorts an array of strings `str` in ascending order based on
 * lexicographical comparison. It uses a bubble sort algorithm to swap adjacent
 *  strings until the array is sorted.
 *
 * @param str The array of strings to be sorted.
 * @param n The number of strings in the array.
 */
static void	ft_sort_str_tab(char **str, int n)
{
	int		k;
	int		j;
	char	*swap;

	k = 0;
	while (k < n - 1)
	{
		j = 0;
		while (j < n - k - 1)
		{
			if (ft_strcmp(str[j], str[j + 1]) > ft_strcmp(str[j + 1], str[j]))
			{
				swap = str[j];
				str[j] = str[j + 1];
				str[j + 1] = swap;
			}
			j++;
		}
		k++;
	}
}

/**
 * @brief Prints a str with double quotes around its value after the '=' symbol
 *
 * This function prints the given string `str` in the format `declare -x <str>`
 * where the portion after the '=' symbol is enclosed in double quotes. This is
 * useful for simulating env variable declarations in a shell-like format.
 *
 * @param str The string to be printed with quoted value after '='.
 */
static void	ft_str_dquotes(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	ft_putstr_fd("declare -x ", 1);
	while (str[i])
	{
		ft_putchar(str[i]);
		if (str[i] == '=' && flag == 0)
		{
			ft_putchar('"');
			flag++;
		}
		i++;
	}
	if (flag == 1)
		ft_putchar('"');
	ft_putchar('\n');
}

/**
 * @brief Prints the environment variables in a sorted, quoted format.
 *
 * This function retrieves a copy of the environment variables (`envp`), 
 * sorts them alphabetically, and prints each variable in the format
 * `declare -x <var>="<value>"`. It uses `ft_str_dquotes` to format each
 * variable with double quotes around the value.
 *
 * @param envp The array of environment variables to be printed.
 */
void	ft_print_export(char **envp)
{
	char	**sorted_envp;
	int		size_envp;
	int		i;

	sorted_envp = ft_get_my_envp(envp);
	size_envp = 0;
	i = 0;
	while (envp[size_envp])
		size_envp++;
	ft_sort_str_tab(sorted_envp, size_envp);
	while (sorted_envp[i])
	{
		ft_str_dquotes(sorted_envp[i]);
		free(sorted_envp[i]);
		i++;
	}
	free(sorted_envp);
	ft_free_vector(envp);
}

/**
 * @brief Validates if each argument is a valid environment variable identifier
 *
 * Checks each string in `argv` to ensure it follows the rules for environment 
 * variable names.
 * The first character must be alphabetic or an underscore;
 * subsequent characters may include alphanumeric, underscore, or a plus sign
 * (`+`) only if it is followed by an equals sign (`=`).
 *
 * @param argv Array of argument strings to validate.
 * @return 0 if all identifiers are valid, or an error message and non-zero
 * 			value if any identifier is invalid.
 */
int	check_key(char **argv)
{
	size_t	i;

	while (*argv)
	{
		i = 0;
		if (!ft_isalpha((*argv)[i]) && (*argv)[i] != '_')
			return (ft_stderror(FALSE, ERROR_IDENTIFIER, (*argv)), 1);
		while ((*argv)[i] != '=' && (*argv)[i])
		{
			if (!ft_isalnum((*argv)[i]) && (*argv)[i] != '_')
			{
				if ((*argv)[i] != '+')
					return (ft_stderror(FALSE, ERROR_IDENTIFIER, (*argv)), 1);
				else if ((*argv)[i] == '+' && (*argv)[i + 1] != '=')
					return (ft_stderror(FALSE, ERROR_IDENTIFIER, (*argv)), 1);
			}
			i++;
		}
		argv++;
	}
	return (0);
}

/**
 * @brief Imports a local environment variable into the global environment.
 *
 * This function searches for a variable in the local environment that matches
 * the provided argument. If a match is found and the variable has a valid 
 * format (with an '=' separator), it adds the variable to the global envp.
 *
 * @param sh The shell structure containing local and global environments.
 * @param arg The variable name to search for in the local environment.
 */
void	ft_local_import(t_shell *sh, char *arg)
{
	int		i;
	int		size;
	char	*var;

	i = 0;
	size = ft_strlen(arg);
	var = NULL;
	while ((sh->local)[i])
	{
		if (ft_strncmp(arg, (sh->local)[i], size) == 0
			&& (sh->local)[i][size] == '=')
		{
			var = (sh->local)[i];
			break ;
		}
		i++;
	}
	if (var)
		add_var(var, ft_argslen(sh->global), &(sh->global));
	else
		add_var(arg, ft_argslen(sh->limbo), &(sh->limbo));
}
