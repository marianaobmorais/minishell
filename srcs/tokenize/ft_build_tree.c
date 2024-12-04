#include "../../includes/minishell.h"

/**
 * @brief Searches for the next pipe token in the list and advances the pointer
 * 
 * Iterates through the token list to find the next pipe ('|') token. If a pipe
 * is found, the list pointer is updated to the token immediately after the
 * pipe and returns true. Otherwise, the pointer reaches the end of the list
 * and returns false.
 * 
 * @param list A double pointer to the token list, updated to the position
 *        after the pipe if found.
 * @return true if a pipe token is found, otherwise false.
 */
static bool	ft_find_next_pipe(t_list **list)
{
	t_token	*token; //update brief

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
		{
			*list = (*list)->next;
			return (true);
		}
		if (token->type == AND || token->type == OR)
			return (false);
		*list = (*list)->next;
	}
	return (false);
}

/**
 * @brief Constructs a binary tree representing a pipeline structure.
 * 
 * Builds a binary tree where each node represents a pipe ('|') and its
 * children represent the commands or branches connected by that pipe. The
 * function recursively processes the token list, skipping invalid tokens and
 * linking branches. 
 * 
 * @param list A double pointer to the token list, updated as tokens are
 *        consumed during tree building.
 * @return A pointer to the root of the constructed binary tree, or NULL if an
 *         error occurs.
 */
void	*ft_build_tree(t_list **list, t_node **parent_node)
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
	if (parent_node)
		node->parent_node = *parent_node;
	else
		node->parent_node = NULL;
	node->left = ft_build_branch(list, NULL, NULL);
	if (!node->left)
		return (NULL);
	if (!list || !*list)
		return ((void *)node);
	if (ft_find_next_pipe(list))
		node->right = ft_build_tree(list, parent_node);
	return ((void *)node);
}
