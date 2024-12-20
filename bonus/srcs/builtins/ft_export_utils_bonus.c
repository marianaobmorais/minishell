/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:27:53 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/20 19:02:03 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Removes a variable from the limbo environment in the shell.
 *
 * This function splits the input string `arg` by the '=' character to extract
 * the variable key. It then iterates through the limbo environment variables
 * in the shell structure and removes the variable matching the key.
 * Memory allocated for the temporary array is freed before returning.
 *
 * @param arg The input string containing the variable to be removed.
 * @param sh A pointer to the `t_shell` structure containing the shell state.
 */
void	ft_clear_limbo(char *arg, t_shell *sh)
{
	int		i;
	int		size;
	char	*key;
	char	**temp;

	i = 0;
	temp = ft_split(arg, '=');
	if (!temp)
		return ;
	key = temp[0];
	size = ft_strlen(key);
	while ((sh->limbo)[i])
	{
		if (ft_strncmp(key, (sh->limbo)[i], size) == 0
			&& ft_strlen(key) == ft_strlen((sh->limbo)[i]))
			delete_var_limbo(key, &(sh->limbo), ft_argslen((sh->limbo)));
		i++;
	}
	ft_free_vector(temp);
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
 *         value if any identifier is invalid.
 */
int	ft_check_keys_argv(char **argv)
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
 * @brief Validates a given string to ensure it is a correct environment
 * variable key.
 *
 * This function checks if the input string `argv` is a valid environment
 * variable key. It ensures the first character is either an alphabetic char
 * or an underscore. It then iterates through the rest of the string, allowing
 * alphanumeric characters, underscores, and a single '+' character followed by
 * an '='.If any invalid character is found, it prints an error message and sets
 * the exit status.
 *
 * @param argv The input string to be validated.
 * @return 0 if the input string is a valid key, otherwise it returns -1 and
 *         sets the exit status.
 */
int	ft_check_key(char *argv)
{
	size_t	i;

	if (argv)
	{
		i = 0;
		if (!ft_isalpha(argv[i]) && argv[i] != '_')
			return (ft_stderror(FALSE, ERROR_IDENTIFIER, argv), \
			ft_exit_status(1, TRUE, FALSE));
		while (argv[i] != '=' && argv[i])
		{
			if (!ft_isalnum(argv[i]) && argv[i] != '_')
			{
				if (argv[i] != '+')
					return (ft_stderror(FALSE, ERROR_IDENTIFIER, argv), \
						ft_exit_status(1, TRUE, FALSE));
				else if (argv[i] == '+' && argv[i + 1] != '=')
					return (ft_stderror(FALSE, ERROR_IDENTIFIER, argv), \
						ft_exit_status(1, TRUE, FALSE));
			}
			i++;
		}
	}
	return (0);
}
