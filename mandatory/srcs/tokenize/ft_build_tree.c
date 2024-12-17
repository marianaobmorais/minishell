/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_build_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:34:44 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 17:35:04 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Validates whether the token list can skip export tokens or not.
 * 
 * This function checks if the given token list contains tokens to process for
 * execution, skipping over consecutive `EXPORT` or `EXPORT_AP` tokens. If 
 * `EXPORT` or `EXPORT_AP` tokens are followed by an `EXEC` token, the
 * validation to skip proceedes. If the list ends or if the first non-export
 * token is not an executable (`EXEC`) or if it is a node (`NODE`), it does not
 * skips the `EXPORT` and `EXPORT_AP` tokens.
 * 
 * @param list A double pointer to the token list to validate.
 * @return `true` if the list should skip until with a valid executable token,
 *         otherwise `false`.
 */
static bool	ft_validate_skip(t_list **list)
{
	t_token	*token;
	t_list	*tmp;

	tmp = *list;
	token = tmp->content;
	while (tmp && (token->type == EXPORT || token->type == EXPORT_AP))
	{
		tmp = tmp->next;
		if (tmp)
			token = tmp->content;
	}
	if (!tmp)
		return (false);
	if (ft_is_token_type(((t_token *)tmp->content), NODE)
		|| !ft_is_token_type(((t_token *)tmp->content), EXEC))
		return (false);
	return (true);
}

/**
 * @brief skips consecutive `EXPORT` tokens in the list.
 * 
 * This function iterates through the token list, advancing the list pointer
 * as long as the current token is of type `EXPORT` or `EXPORT_AP`. It stops
 * when a non-EXPORT token is encountered or if the next token is invalid,
 * ensuring that the list points to a token that is not of these types before
 * proceeding.
 * 
 * @param list A pointer to the token list, which will be updated to skip over
 *        any `EXPORT` or `EXPORT_AP` tokens.
 */
static void	ft_skip_export_tokens(t_list **list)
{
	t_token	*token;

	token = (*list)->content;
	while (*list && (token->type == EXPORT || token->type == EXPORT_AP))
	{
		if (!((*list)->next)
			|| ft_is_token_type(((t_token *)(*list)->next->content), NODE))
			break ;
		else
		{
			*list = (*list)->next;
			token = (*list)->content;
		}
	}
}

/**
 * @brief Searches for the next PIPE token in the provided token list.
 *
 * This function iterates through the given token list to locate the next token 
 * of type `PIPE`. If a `PIPE` token is found, the pointer to the list is
 * updated to the node following the `PIPE`, and the function returns `true`. If
 * no `PIPE` is found, it continues to the end of the list and returns `false`.
 *
 * @param list A double pointer to the token list being searched. 
 *        The pointer is updated to point to the node following the `PIPE` if
 *        found.
 * @return `true` if a `PIPE` token is found, `false` otherwise.
 */
static bool	ft_find_next_pipe(t_list **list)
{
	t_token	*token;

	if (!list || !*list)
		return (false);
	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
		{
			*list = (*list)->next;
			return (true);
		}
		*list = (*list)->next;
	}
	return (false);
}

/**
 * @brief Constructs a binary tree representing the shell command structure.
 *
 * This function builds a binary tree from the provided token list to represent 
 * a command's execution structure. Each node in the tree corresponds to a
 * command, redirection, or pipeline segment. The left child of the node
 * represents the current command or redirection branch, while the right child
 * represents subsequent commands in a pipeline if a `PIPE` token is encountered
 *
 * The function handles `PIPE` tokens to split the tree into segments. If
 * export-related tokens are detected and need skipping, they are processed by
 * `ft_skip_export_tokens`.
 *
 * @param list A double pointer to the token list used for tree construction.
 *        The pointer is updated as tokens are consumed.
 * @return A pointer to the root node of the constructed tree, or `NULL` on
 *         failure.
 */
void	*ft_build_tree(t_list **list)
{
	t_node	*node;

	if (ft_validate_skip(list))
		ft_skip_export_tokens(list);
	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (ft_error_malloc("node"), NULL);
	node->type = PIPE;
	node->right = NULL;
	node->left = NULL;
	node->left = ft_build_branch(list, NULL);
	if (!node->left)
		return (NULL);
	if (!list || !*list)
		return ((void *)node);
	if (ft_find_next_pipe(list))
		node->right = ft_build_tree(list);
	return ((void *)node);
}
