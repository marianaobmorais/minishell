/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:27:46 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/26 17:35:48 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Adds a new environment variable to the environment array.
 *
 * Allocates space for a new env array with one additional slot for `str`,
 * copying existing variables from `envp` and appending the new variable.
 *
 * @param str The new environment variable string to add.
 * @param size The current number of environment variables in `envp`.
 * @param envp Pointer to the environment variable array to be updated.
 */
int	add_var(char *str, size_t size, char ***envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	new_envp = (char **) malloc((size + 2) * sizeof(char *));
	if (!new_envp)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), -1);
	i = 0;
	while ((*envp)[i])
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	new_envp[i] = ft_strdup(str);
	new_envp[i + 1] = NULL;
	free((*envp));
	*envp = new_envp;
	return (0);
}

/**
 * @brief Replaces or adds an environment variable in the environment array.
 *
 * This function updates an existing environment variable if its key matches the
 * given string. If the variable is not found in the environment array:
 * - Returns -1 if the mode is `LOCAL`.
 * - Clears temporary memory associated with the variable (if mode is `DEFAULT`)
 *   and adds the variable to the environment array.
 * The function ensures proper memory management for replaced or newly added
 * variables.
 *
 * @param str The environment variable in the format "KEY=VALUE".
 * @param envp A pointer to the environment array.
 * @param mode Determines whether to add a variable if it does not exist:
 *        - `LOCAL`: Do not add, return -1.
 *        - `DEFAULT`: Add the variable if it does not exist.
 * @param sh A pointer to the shell structure, used for clearing temporary
 *        memory.
 * @return 0 if the variable was successfully replaced or added. And -1 if the
 *         variable was not found and the mode is `LOCAL`.
 */
int	replace_var(char *str, char ***envp, t_env mode, t_shell *sh)
{
	int	i;
	int	size;

	i = 0;
	size = (ft_strlen(str) - ft_strlen(ft_strchr(str, '=')));
	while ((*envp)[i])
	{
		if (ft_strncmp(str, (*envp)[i], size) == 0
			&& (*envp)[i][size] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(str);
			return (0);
		}
		i++;
	}
	if (mode == LOCAL)
		return (-1);
	if (mode == DEFAULT)
		ft_clear_limbo(str, sh);
	return (add_var(str, i, envp));
}

/**
 * @brief Concatenates a variable with an existing environment variable or adds
 * a new variable if not present.
 *
 * This function searches for an environment variable matching the given string
 * and concatenates its value if found. If the variable is not found and the
 * mode is LOCAL or LIMBO, it returns -1. In DEFAULT mode, it calls 
 * `ft_concat_plus` to handle the concatenation. If the variable is still not
 * found, it trims the '+' character and adds the new variable to the
 * environment.
 *
 * @param str The input string containing the variable to concatenate.
 * @param envp A pointer to the environment variables array.
 * @param mode The mode of the environment (LOCAL, LIMBO, or DEFAULT).
 * @param sh A pointer to the t_shell structure containing the shell state.
 * @return 0 on success, -1 if the variable is not found in LOCAL or LIMBO mode.
 */
int	concatenate_var(char *str, char ***envp, t_env mode, t_shell *sh)
{
	int		i;
	int		size;
	char	*temp_str;
	char	*value;

	i = 0;
	while ((*envp)[i])
	{
		value = ft_strchr(str, '+') + 2;
		size = (ft_strlen(str) - ft_strlen(value)) - 2;
		if (ft_strncmp(str, (*envp)[i], size) == 0 && (*envp)[i][size] == '=')
		{
			temp_str = ft_strjoin((*envp)[i], value);
			free((*envp)[i]);
			(*envp)[i] = temp_str;
			return (0);
		}
		i++;
	}
	if (mode == LOCAL || mode == LIMBO)
		return (-1);
	if (mode == DEFAULT && ft_concat_plus(str, DEFAULT, sh) == 0)
		return (0);
	temp_str = ft_chartrim(str, '+');
	return (add_var(temp_str, i, envp), free(temp_str), 0);
}

/**
 * @brief Exports local shell variables.
 *
 * Processes and exports local shell variables. It checks if the keys are valid
 * and contain '='.If the variable contains '+', it concatenates; otherwise, it
 * replaces the variable. It updates either the global or local shell variables
 * based on the provided arguments.
 *
 * @param argv An array of strings representing the arguments.
 * @param sh The shell structure containing the global and local environments.
 *
 * @return 0 on success, or an exit status code for various error conditions.
 */
static int	ft_export_local(char **argv, t_shell *sh)
{
	size_t	s_key;

	if (ft_check_keys_argv(argv) != 0)
		return (ft_exit_status(1, TRUE, FALSE));
	while (*argv)
	{
		if (!ft_strchr(*argv, '='))
			return (ft_exit_status(0, TRUE, FALSE));
		s_key = (ft_strlen(*argv) - ft_strlen(ft_strchr(*argv, '=')));
		if ((*argv)[s_key - 1] == '+')
		{
			if (concatenate_var(*argv, &(sh->global), LOCAL, NULL) == -1
				&& ft_limbo_import(sh, *argv) == -1)
				concatenate_var(*argv, &(sh->local), DEFAULT, sh);
		}
		else
		{
			if (replace_var(*argv, &(sh->global), LOCAL, sh) == -1
				&& ft_limbo_import(sh, *argv) == -1)
				replace_var(*argv, &(sh->local), DEFAULT, sh);
		}
		argv++;
	}
	return (ft_exit_status(0, TRUE, FALSE));
}

/**
 * @brief Manages the exportation of environment variables.
 *
 * Handles environment variable exportation. If no arguments are provided,
 * it prints the current environment variables. If arguments are passed, it
 * validates the keys and either concatenates or replaces the variables in
 * the global environment. Supports local exportation mode if specified.
 *
 * @param argc The number of arguments passed to the function.
 * @param argv An array of strings representing the arguments.
 * @param sh The shell structure containing the global and local environments.
 * @param mode The export mode, either LOCAL or GLOBAL.
 *
 * @return 0 on success, or an exit status code for various error conditions.
 */
int	ft_export(int argc, char **argv, t_shell *sh, t_env mode)
{
	size_t	s_key;

	if (mode == LOCAL)
		return (ft_export_local(argv, sh));
	if (argc == 1)
		return (ft_print_export(ft_merge_env(sh->global, sh->limbo)), \
			ft_exit_status(0, TRUE, FALSE));
	argv++;
	ft_exit_status(0, TRUE, FALSE);
	while (*argv)
	{
		if (ft_check_key(*argv) == 0)
		{
			s_key = (ft_strlen(*argv) - ft_strlen(ft_strchr(*argv, '=')));
			if (!ft_strchr(*argv, '='))
				ft_local_import(sh, *argv);
			else if (s_key > 0 && (*argv)[s_key - 1] == '+')
				concatenate_var(*argv, &(sh->global), DEFAULT, sh);
			else
				replace_var(*argv, &(sh->global), DEFAULT, sh);
		}
		argv++;
	}
	return (ft_exit_status(0, FALSE, FALSE));
}
