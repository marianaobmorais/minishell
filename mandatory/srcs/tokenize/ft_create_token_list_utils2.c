/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_token_list_utils2.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:43:24 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 17:43:25 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Determines if the last added token to a list is a HEREDOC token.
 * 
 * This function checks the last added node to the provided `list` and verifies 
 * if its content represents a HEREDOC token. If the last node is not a HEREDOC
 * token, the function returns `false`, else, it returns `true`.
 * 
 * @param list A pointer to the list of tokens.
 * @return `true` if the last token in the list is of type `HEREDOC`, 
 *         otherwise `false`.
 */
bool	ft_is_heredoc_target(t_list **list)
{
	t_list	*last;
	t_token	*token;

	last = ft_lstlast(*list);
	if (!last || !last->content)
		return (false);
	token = (t_token *)last->content;
	if (token->type == HEREDOC)
		return (true);
	return (false);
}
