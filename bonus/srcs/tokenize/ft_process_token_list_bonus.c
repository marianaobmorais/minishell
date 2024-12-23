/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_token_list_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:25:39 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/22 16:51:46 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Replaces a token in the token list with a list of wildcard-expanded
 *        tokens.
 * 
 * This function replaces a given node (`curr`) in a linked list with a list of 
 * expanded wildcard tokens (`wild_list`). It correctly adjusts the `prev`
 * pointer and updates the `head` pointer if the replaced node is the head of
 * the list. Any remaining nodes after `curr` are linked to the end of the new
 * `wild_list`.
 * 
 * @param curr The current node in the token list being replaced. Its content 
 *        will be freed, and the node will be removed.
 * @param prev The previous node in the list before `curr`. This is updated to 
 *        point to the first node of `wild_list`. If `curr` is the head, this 
 *        parameter should be `NULL`.
 * @param head A pointer to the head of the list. This will be updated if `curr`
 *        is the first node.
 * @param wild_list The list of expanded wildcard tokens to replace `curr`.
 *        This list is linked into the main token list.
 */
static void	ft_update_token_list(t_list *curr, t_list *prev, t_list **head, \
	t_list **wild_list)
{
	t_list	*next;
	t_list	*last_node;

	next = curr->next;
	ft_free_content(curr);
	if (prev)
		prev->next = *wild_list;
	else
		*head = *wild_list;
	if (*wild_list)
	{
		last_node = ft_lstlast(*wild_list);
		last_node->next = next;
	}
}

/**
 * @brief Handles wildcard expansion for the current token in the token list.
 *
 * This function processes a token containing a wildcard character (`*`),
 * expands it into a list of matching entries (files or directories), and
 * updates the token list by replacing the current token with the expanded
 * entries.
 *
 * @param current  double pointer to the current position in the token list.
 *        This pointer may be updated to point to the new list of tokens if a
 *        wildcard is expanded.
 * @param prev A pointer to the previous node in the token list, used for
 *        relinking after replacement.
 * @param head A double pointer to the head of the token list. This is updated
 *        if the head is replaced.
 * @return 
 * - `true` if wildcard expansion was successfully performed and the token list
 *   was updated.
 * - `false` otherwise.
 */
static bool	ft_handle_wildcard(t_list **current, t_list *prev, t_list **head)
{
	t_list	**wild_list;
	t_token	*token;
	bool	flag;

	flag = false;
	token = (t_token *)(*current)->content;
	wild_list = ft_get_wildcard_list(token->value);
	if (wild_list && *wild_list)
	{
		ft_update_token_list(*current, prev, head, wild_list);
		flag = true;
		*current = *wild_list;
	}
	if (wild_list)
		free(wild_list);
	return (flag);
}

/**
 * @brief Removes a specified node from a linked list.
 * 
 * Detaches the `current` node from the linked list, adjusting the pointers
 * of the previous node (`prev`) or the head of the list if `current` is the
 * first node. Frees the memory allocated to the `current` node using
 * `ft_free_node`.
 * 
 * @param list Double pointer to the head of the list.
 * @param prev Pointer to the previous node, or NULL if `current` is the head.
 * @param curr Pointer to the node to be removed.
 */
void	ft_remove_current_node(t_list **list, t_list *prev, t_list *curr)
{
	t_list	*next;

	next = NULL;
	next = curr->next;
	if (prev)
		prev->next = next;
	else
		*list = next;
	ft_free_content(curr);
}

/**
 * @brief Processes token expansion and removes quotes from a token.
 *
 * This function handles two main operations for a given token:
 * 1. Expands environment variables within the token's value if expansion is
 *    enabled.
 * 2. Removes surrounding or embedded quotes (`'` or `"`) if the token is in a
 *    quoted state.
 *
 * @param token A pointer to the token structure to be processed.
 * @param envp A null-terminated array of strings representing the environment
 *        variables.
 */
static void	ft_process_expansion_and_quotes(t_token *token, char **envp)
{
	if (token->expand)
		ft_expand_tokens(token, envp);
	if (token->state == IN_QUOTE)
		ft_remove_quotes(token);
}

/**
 * @brief Processes a linked list of tokens, expanding variables,
 *        handling wildcards, and removing empty nodes as necessary.
 *
 * This function iterates through a list of tokens, applying token-specific
 * transformations such as environment variable expansion, quote removal, and
 * wildcard processing. Empty tokens generated after expansion that are in
 * `IN_QUOTE` state are removed from the list.
 *
 * @param list A pointer to the head of the linked list of tokens to be
 *        processed.
 * @param envp A null-terminated array of strings representing the environment
 *        variables.
 */
void	ft_process_token_list(t_list **list, char **envp)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;
	t_token	*token;

	current = *list;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		ft_process_expansion_and_quotes(token, envp);
		if (!*token->value && token->expand && !token->state)
		{
			ft_remove_current_node(list, prev, current);
			current = next;
			continue ;
		}
		if (token->wildcard && ft_handle_wildcard(&current, prev, list))
			prev = NULL;
		else
			prev = current;
		current = current->next;
	}
	ft_free_vector(envp);
}
