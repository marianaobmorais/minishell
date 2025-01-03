/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_build_branch_utils2_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:04:44 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 19:06:39 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Creates a sub-list of tokens enclosed within parentheses.
 * 
 * This function extracts a sub-list of tokens from a token list, starting 
 * from the current position and including all tokens up to, but not 
 * including, the closing parenthesis `)` that matches the opening 
 * parenthesis `(` before the current position.
 * The function maintains a nesting count (`count`) to handle nested 
 * parentheses correctly, ensuring that the correct closing parenthesis 
 * is matched.
 * The extracted tokens are stored in a new list (`t_list **`), which is 
 * dynamically allocated and returned to the caller.
 * 
 * @param list A pointer to the list of tokens. This pointer will be updated 
 *        to point to the token immediately following the closing parenthesis.
 * @return A pointer to a new list (`t_list **`) containing the sub-list 
 *         of tokens, or `NULL` if memory allocation fails.
 */
static t_list	**ft_create_sub_list(t_list **list)
{
	t_list	**sub;
	t_token	*token;
	int		count;

	sub = (t_list **)malloc(sizeof(t_list *));
	if (!sub)
		return (ft_error_malloc("sub"), NULL);
	*sub = NULL;
	count = 1;
	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PRTHESES && token->value[0] == '(')
			count++;
		if (token->type == PRTHESES && token->value[0] == ')')
			count--;
		if (count == 0)
		{
			*list = (*list)->next;
			break ;
		}
		ft_add_to_token_list(&token->value, sub);
		*list = (*list)->next;
	}
	return (sub);
}

/**
 * @brief Creates a subroot node for a subtree from a sub-list of tokens.
 * 
 * This function processes a subexpression enclosed in parentheses, 
 * extracting the tokens between the parentheses, and constructing a 
 * subtree for those tokens. It skips the opening parenthesis `(` and 
 * processes the tokens up to, but not including, the matching closing 
 * parenthesis `)`.
 * The tokens within the parentheses are extracted into a sub-list using 
 * `ft_create_sub_list`, which is then passed to `ft_build_root` to build 
 * a subtree. The created subtree is encapsulated in a subroot node of 
 * type `t_node`.
 * 
 * @param list A pointer to the current token list. This pointer will 
 *        be updated to point to the token immediately following the 
 *        closing parenthesis `)`.
 * @return A pointer to the newly created `t_node` representing the 
 *         root of the subtree, or `NULL` if memory allocation fails 
 *         or the sub-list is invalid.
 */
t_node	*ft_create_subroot_node(t_list **list)
{
	t_node	*sub_root;
	t_list	**sub_list;
	t_list	*head;

	sub_root = NULL;
	*list = (*list)->next;
	sub_list = ft_create_sub_list(list);
	if (!sub_list || !*sub_list)
		return (NULL);
	head = *sub_list;
	sub_root = ft_build_root(sub_list, SUB_ROOT);
	ft_free_list(head);
	free(sub_list);
	return (sub_root);
}

/**
 * @brief Searches for the next redirection token in the token list.
 * 
 * This function iterates through a token list, starting from the current
 * position, to locate the next token of type `REDIR`. The search stops at
 * tokens of type `NODE` or at the end of the list. If a redirection token is
 * found, the function returns `true`, and the pointer to the list is updated to
 * the position of the found token. Otherwise, it returns `false`.
 * 
 * @param list A double pointer to the current position in the token list. 
 *        If a redirection token is found, the pointer will point to it.
 * @return `true` if a redirection token is found; `false` otherwise.
 */
bool	ft_find_next_redir(t_list **list)
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (ft_is_token_type(token, NODE))
			break ;
		if (ft_is_token_type(token, REDIR))
			return (true);
		*list = (*list)->next;
	}
	return (false);
}
