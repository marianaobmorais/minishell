/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_lvl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:13:01 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/26 17:33:38 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Validates if a string represents a valid positive numeric value.
 *
 * This function determines whether the given string conforms to the format
 * of a valid positive integer. It allows an optional leading '+' character,
 * followed by one or more digits. Negative signs ('-') and non-digit
 * characters render the string invalid. Strings with an optional sign
 * but no digits (e.g., "+" or "-") are also considered invalid.
 *
 * @param arg The string to validate.
 * @return true if the string represents a valid positive integer.
 * @return false if the string does not represent a valid positive integer.
 */
static bool	ft_is_valid_numeric(char *arg)
{
	int	i;

	if (!arg)
		return (false);
	i = 0;
	if (arg[i] == '-' )
		return (false);
	if (arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	if (arg[0] == '\0' || ((arg[0] == '-' || arg[0] == '+')
			&& !ft_isdigit(arg[1])))
		return (false);
	return (true);
}

/**
 * @brief Handles the increment and update of the shell level (SHLVL) in the
 *        environment.
 *
 * This function retrieves the current `SHLVL` value from the shell's global
 * environment. If it does not exist, it initializes it to 1. Otherwise, it
 * increments the level by 1, updates the environment, and frees any allocated
 * memory used during the operation.
 *
 * @param sh A pointer to the shell structure containing the global environment.
 */
void	ft_handle_lvl(t_shell *sh)
{
	char	*shlvl;
	char	*new_shlvl;
	char	*n;
	int		lvl_n;

	shlvl = ft_getenv("SHLVL=", sh->global);
	if (!shlvl || !ft_is_valid_numeric(shlvl))
		replace_var("SHLVL=1", &(sh->global), DEFAULT, sh);
	else
	{
		lvl_n = ft_atoi(shlvl) + 1;
		n = ft_itoa(lvl_n);
		if (!n)
			return (ft_error_malloc("SHLVL"));
		new_shlvl = ft_strjoin("SHLVL=", n);
		if (!new_shlvl)
			return (free(n), ft_error_malloc("SHLVL update"));
		replace_var(new_shlvl, &(sh->global), DEFAULT, sh);
		free(shlvl);
		free(new_shlvl);
		free(n);
	}
}
