/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_manager_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:41:27 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:41:28 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_bonus.h"

/**
 * @brief Merges global and local environment variables into a single array.
 *
 * Combines the environment variables stored in `sh->global` and `sh->local`
 * into a newly allocated array. Each variable is duplicated, and memory
 * allocation is handled. The resulting array ends with a NULL pointer.
 *
 * @param sh A pointer to the shell structure containing the `global` and
 *           `local` environment variable arrays.
 * @return A new array containing all environment variables, or NULL if
 *         a memory allocation error occurs.
 */
char	**ft_merge_env(char **env1, char **env2)
{
	int		size;
	int		i;
	int		z;
	char	**envp;

	z = 0;
	i = 0;
	size = ft_argslen(env1) + ft_argslen(env2);
	envp = (char **) malloc((size + 1) * sizeof(char *));
	if (!envp)
		return (ft_error_malloc("envp"), NULL);
	while (env2[i])
	{
		envp[i] = ft_strdup(env2[i]);
		i++;
	}
	if (env1)
	{
		while (env1[z])
		{
			envp[i++] = ft_strdup(env1[z++]);
		}
	}
	envp[i] = NULL;
	return (envp);
}

/**
 * @brief Creates a duplicate of the environment variables array.
 *
 * Allocates memory for a new array and copies each string from the
 * input `envp` array into it. Each string is duplicated, and the
 * resulting array is NULL-terminated.
 *
 * @param envp The original environment variables array to be duplicated.
 * @return A new array containing duplicates of all strings in `envp`, or
 *         NULL if a memory allocation error occurs.
 */
char	**ft_get_my_envp(char **envp)
{
	char	**my_envp;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	my_envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!my_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		my_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}
