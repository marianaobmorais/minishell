/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:31:38 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:31:39 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
 * @brief Replaces an existing env variable or adds a new one if not found.
 *
 * Searches for a variable in `envp` matching `str` up to the equal sign `=`
 * If found, frees the old entry and replaces it with `str`. If not found, 
 * calls `add_var` to add `str` as a new environment variable.
 *
 * @param str The environment variable string to replace or add.
 * @param size The length of the variable name in `str` (up to `=`).
 * @param envp Pointer to the array of environment variables to be updated.
 * @return Returns 0 if the variable is replaced or added new, if LOCAL mode
 *         Returns -1 if not replaced.
 */
static int	replace_var(char *str, size_t size, char ***envp, t_env mode)
{
	int	i;

	i = 0;
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
	return (add_var(str, i, envp));
}

/**
 * @brief Concatenates a value to an existing environment variable or 
 *        adds it as a new variable.
 *
 * Searches for an environment variable in `envp` that matches the prefix of
 * `str` (before `+=`). If found, concatenates the new value from
 * `str` (after `+=`) to the existing variable value.
 * If not found, calls `add_var` to add `str` as a new environment variable.
 *
 * @param str The environment variable string with the format "VAR+=VALUE".
 * @param envp Pointer to the array of environment variables to be updated.
 * @return Returns 0 if the var is concatenated or added new, if LOCAL mode
 *         Returns -1 if not concatenated.
 */
static int	concatenate_var(char *str, char ***envp, t_env mode)
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
		if (ft_strncmp(str, (*envp)[i], size) == 0
			&& (*envp)[i][size] == '=')
		{
			temp_str = ft_strjoin((*envp)[i], value);
			free((*envp)[i]);
			(*envp)[i] = temp_str;
			return (0);
		}
		i++;
	}
	if (mode == LOCAL)
		return (-1);
	return (add_var(str, i, envp));
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

	if (check_key(argv) != 0)
		return (ft_exit_status(2, TRUE, FALSE));
	while (*argv)
	{
		if (!ft_strchr(*argv, '='))
			return (ft_exit_status(0, TRUE, FALSE));
		s_key = (ft_strlen(*argv) - ft_strlen(ft_strchr(*argv, '=')));
		if ((*argv)[s_key - 1] == '+')
		{
			if (concatenate_var(*argv, &(sh->global), LOCAL) == -1
				&& ft_limbo_import(sh, *argv) == -1)
				concatenate_var(*argv, &(sh->local), DEFAULT);
		}
		else
		{
			if (replace_var(*argv, s_key, &(sh->global), LOCAL) == -1
				&& ft_limbo_import(sh, *argv) == -1)
				replace_var(*argv, s_key, &(sh->local), DEFAULT);
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
	int		exit_code;

	if (mode == LOCAL)
		return (ft_export_local(argv, sh));
	if (argc == 1)
	{
		ft_print_export(ft_merge_env(sh->global, sh->limbo));
		return (ft_exit_status(0, TRUE, FALSE));
	}
	argv++;
	exit_code = check_key(argv);
	while (*argv)
	{
		s_key = (ft_strlen(*argv) - ft_strlen(ft_strchr(*argv, '=')));
		if (!ft_strchr(*argv, '='))
			ft_local_import(sh, *argv);
		else if (s_key > 0 && (*argv)[s_key - 1] == '+')
			concatenate_var(*argv, &(sh->global), DEFAULT);
		else
			replace_var(*argv, s_key, &(sh->global), DEFAULT);
		argv++;
	}
	return (ft_exit_status(exit_code, TRUE, FALSE));
}
