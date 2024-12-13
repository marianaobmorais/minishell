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
 * @brief Locates the next pipe token in the token list.
 * 
 * This function searches through a linked list of tokens, starting from the 
 * given node, for the next token of type `PIPE`. If a `PIPE` token is found, 
 * the list pointer is updated to the node following the `PIPE`, and the 
 * function returns `true`. If a logical operator token (`AND` or `OR`) is 
 * encountered before finding a `PIPE`, the search stops, and the function 
 * returns `false`. If no `PIPE` is found and the list is exhausted, 
 * `false` is returned.
 * 
 * @param list Pointer to a pointer to the current node in the token list. 
 *        The function modifies this pointer as it iterates through the list.
 * @return `true` if a `PIPE` token is found; otherwise, `false`.
 */
static bool	ft_find_next_pipe(t_list **list)
{
	t_token	*token; //update brief

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
 * @brief Constructs a binary tree representing a pipeline structure.
 *
 * This function recursively builds a binary tree from a list of tokens, where 
 * each node represents a segment of a pipeline or command sequence. The tree
 * is built by processing branches and identifying pipeline operators (`|`) to
 * split the structure into left and right subtrees. Each node points to its
 * parent node, enabling traversal up the tree.
 *
 * @param list A double pointer to the head of the token list. The list is
 *        updated as tokens are processed.
 * @param parent_node A pointer to the parent node for the current subtree. If 
 *        `NULL`, the current node is the root of the tree.
 * @return A pointer to the root of the newly constructed subtree, or `NULL` if 
 *         memory allocation fails or if the token list is empty or invalid.
 */
void	*ft_build_tree(t_list **list)
{
	t_node	*node; //update brief

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
