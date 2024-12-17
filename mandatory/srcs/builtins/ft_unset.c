/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:31:41 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:31:42 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if a variable exists in the environment.
 *
 * This function iterates through the `envp` environment array to check if 
 * a variable with the name `str` exists. It compares `str` with each env
 * variable up to the length of `str` and ensures an exact match followed
 * by an '='.
 * If the variable is found, it returns the index of the environment variable.
 * Otherwise, it returns 0.
 *
 * @param str The variable name to search for.
 * @param envp A pointer to the array of environment variables.
 * @return The length of the environment variable; otherwise, 0.
 */
static size_t	is_var(char *str, char **envp)
{
	size_t	index_envp;
	size_t	flag;
	size_t	size_var;

	flag = 0;
	index_envp = 0;
	size_var = ft_strlen(str);
	if (!envp || !*envp)
		return (0);
	while ((envp)[index_envp])
	{
		if (ft_strncmp(str, (envp)[index_envp], size_var) == 0
			&& (envp)[index_envp][size_var] == '=')
			flag++;
		index_envp++;
	}
	if (flag > 0)
		return (index_envp);
	else
		return (0);
}

/**
 * @brief Deletes a variable from the environment.
 *
 * This function removes a variable with the name `str` from the `envp` 
 * environment array. It compares `str` with each environment variable up to 
 * the length of `str` and ensures an exact match followed by an '='.
 * If a match is found, the variable is removed and the memory is freed.
 * A new environment array is created without the removed variable and the
 * old environment array is freed.
 *
 * @param str The name of the variable to be deleted.
 * @param envp A pointer to the environment variable array.
 * @param size_env The size of the environment array.
 * @return 0 on success, or -1 if memory allocation fails.
 */
static int	delete_var(char *str, char ***envp, size_t size_env)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	**new_envp;

	size = ft_strlen(str);
	i = 0;
	j = 0;
	new_envp = (char **) malloc((size_env + 1) * sizeof(char *));
	if (!new_envp)
		return (ft_error_malloc("new_envp"), -1);
	while (i < size_env)
	{
		if (ft_strncmp(str, (*envp)[i], size) == 0
			&& (*envp)[i][size] == '=')
		{
			free((*envp)[i]);
			i++;
		}
		new_envp[j++] = (*envp)[i++];
	}
	new_envp[j] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}

/**
 * @brief Deletes a specified variable from the environment.
 *
 * Checks if the variable `argv` exists in the environment array `envp`.
 * If found, removes the variable by reallocating the array and shifting the
 * remaining variables.
 * 
 * @param argv The name of the variable to delete.
 * @param envp A pointer to the environment array.
 */
static void	ft_delete(char *argv, char ***envp)
{
	size_t	size_env;

	if ((*envp))
	{
		size_env = is_var(argv, *envp);
		if (size_env > 0)
			delete_var(argv, envp, size_env);
	}
}

/**
 * @brief Unsets specified variables from the shell environment.
 *
 * Iterates through the provided arguments and removes each variable from the
 * global, local, and limbo environment arrays if they exist.
 *
 * @param argc The number of arguments provided.
 * @param argv An array of argument strings, where each argument is the name of
 *        a variable to unset.
 * @param sh A pointer to the shell structure containing the global, local, and
 *        limbo environments.
 * @return Always returns a success exit status (0).
 */
int	ft_unset(int argc, char **argv, t_shell *sh)
{
	if (argc == 1)
		return (ft_exit_status(0, TRUE, FALSE));
	++argv;
	while (*argv)
	{
		ft_delete(*argv, &(sh->global));
		ft_delete(*argv, &(sh->local));
		ft_delete(*argv, &(sh->limbo));
		argv++;
	}
	return (ft_exit_status(0, TRUE, FALSE));
}
