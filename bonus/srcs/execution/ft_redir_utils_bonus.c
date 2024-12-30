/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 15:08:43 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/30 15:09:51 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Checks if the given string consists entirely of asterisks ('*').
 *
 * This function iterates through the given string and verifies whether 
 * all characters are asterisks. If the string is composed entirely of asterisks 
 * or is empty after skipping leading asterisks, it returns true.
 *
 * @param s The input string to check.
 * @return true if the string consists only of asterisks or is empty.
 * @return false if there are non-asterisk characters in the string.
 */
bool	ft_is_star(char *s)
{
	int	i;

	i = 0;
	while (s[i] == '*')
		i++;
	if (!s[i])
		return (true);
	else
		return (false);
}
