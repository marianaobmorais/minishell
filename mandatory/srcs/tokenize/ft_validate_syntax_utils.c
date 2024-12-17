/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate_syntax_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:49:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 18:49:39 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if the given character indicates the start of a comment.
 * 
 * This function determines if a character is the start of a comment (e.g.,
 * `#`). If a comment is detected, the index pointer is incremented to skip
 * the character, and the function returns `true`. Otherwise, it returns
 * `false`.
 * 
 * @param c The character to check.
 * @param i Pointer to the current index in the input string, updated if a
 *        comment is detected.
 * @return `true` if the character indicates the start of a comment, otherwise
 *         `false`.
 */
bool	ft_is_comment(char c, int *i)
{
	if (c == '#')
	{
		(*i)++;
		return (true);
	}
	return (false);
}
