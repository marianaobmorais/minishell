/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_ultimate_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:27:50 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/19 07:29:01 by joneves-         ###   ########.fr       */
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
 * @brief Removes all occurrences of a specified character from a string.
 *
 * This function creates a new string by removing all occurrences of the 
 * specified character from the input string. If the input string is NULL, the 
 * function returns NULL.
 * If the specified character is not found in the input string, the original
 * string is returned. The function handles memory allocation for the new
 * string and ensures proper error handling in case the allocation fails.
 *
 * @param str The input string from which the character will be removed.
 * @param c The character to be removed from the string.
 * @return A new string with all occurrences of the specified character removed,
 *         or the original string if the character is not found.
 */
char	*ft_chartrim(char *str, char c)
{
	char	*new_str;
	int		i;

	if (!str)
		return (NULL);
	if (!ft_strchr(str, c))
		return (str);
	i = 0;
	new_str = malloc(ft_strlen(str) * sizeof(char));
	if (!new_str)
		ft_error_malloc("chartrim");
	while (*str)
	{
		if (*str == c)
			str++;
		if (*str != c)
		{
			new_str[i] = *str;
			i++;
		}
		str++;
	}
	new_str[i] = '\0';
	return (new_str);
}

/**
 * @brief Concatenates a variable with the '+' symbol in the shell environment.
 *
 * This function processes a string containing a variable with a '+' symbol.
 * It trims the '+' symbol from the string and attempts to import the variable
 * into the shell environment. If the variable is not already present, it
 * splits the string by the '+' symbol and concatenates the variable to the
 * shell's local environment. The function handles memory allocation and
 * deallocation for the temporary strings and arrays.
 *
 * @param str The input string containing the variable with the '+' symbol.
 * @param mode The mode of the environment (unused).
 * @param sh A pointer to the t_shell structure containing the shell state.
 * @return 0 on success, -1 on failure.
 */
int	ft_concat_plus(char *str, t_env mode, t_shell *sh)
{
	char	**str_key;
	char	*temp_str;

	(void)mode;
	temp_str = ft_chartrim(str, '+');
	if (ft_limbo_import(sh, temp_str) == 0)
	{
		free(temp_str);
		return (0);
	}
	str_key = ft_split(str, '+');
	if (concatenate_var(str, &(sh->local), LOCAL, sh) == 0)
	{
		free(temp_str);
		ft_local_import(sh, str_key[0]);
		ft_free_vector(str_key);
		return (0);
	}
	ft_free_vector(str_key);
	free(temp_str);
	return (-1);
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
