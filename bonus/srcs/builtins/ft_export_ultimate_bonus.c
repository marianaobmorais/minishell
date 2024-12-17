/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_ultimate_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:27:50 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:27:51 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Deletes a variable from an environment array in limbo storage.
 *
 * Searches for a variable in the environment array `envp` matching the string
 * `str` up to its length. Removes the matching variable, shifts the remaining
 * variables, and reallocates the array. Frees the old array and updates the
 * pointer.
 *
 * @param str The name of the variable to delete.
 * @param envp A pointer to the environment array.
 * @param size_env The current size of the environment array.
 * @return 0 on success, -1 on memory allocation failure.
 */
static int	delete_var_limbo(char *str, char ***envp, size_t size_env)
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
		if (ft_strncmp(str, (*envp)[i], size) == 0)
		{
			free((*envp)[i]);
			i++;
		}
		new_envp[j++] = (*envp)[i++];
	}
	new_envp[j] = NULL;
	free((*envp));
	*envp = new_envp;
	return (0);
}

/**
 * @brief Imports a variable from limbo storage to the global environment.
 *
 * Searches for a variable in the shell's `limbo` storage that matches `arg`.
 * If found, adds the variable to the global environment and deletes it from
 * `limbo`. If no matching variable is found, returns an error.
 *
 * @param sh The shell structure containing the `limbo` and `global`
 *        environments.
 * @param arg The variable to search for in `limbo`.
 * @return 0 on successful import, -1 if the variable is not found.
 */
int	ft_limbo_import(t_shell *sh, char *arg)
{
	int		i;
	int		size;
	char	*var;

	i = 0;
	var = NULL;
	while ((sh->limbo)[i])
	{
		size = ft_strlen((sh->limbo)[i]);
		if (ft_strncmp(arg, (sh->limbo)[i], size) == 0
			&& arg[size] == '=')
		{
			var = (sh->limbo)[i];
			break ;
		}
		i++;
	}
	if (var)
	{
		add_var(arg, ft_argslen(sh->global), &(sh->global));
		delete_var_limbo(var, &(sh->limbo), ft_argslen((sh->limbo)));
		return (0);
	}
	return (-1);
}
