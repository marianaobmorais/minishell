#include "../../includes/minishell.h"

bool	ft_validate_skip(t_list **list)
{
	t_token	*token;
	t_list	*tmp;

	tmp = *list;
	token = tmp->content;
	while (tmp && (token->type == EXPORT || token->type == EXPORT_AP))
	{
		tmp = tmp->next;
		if(tmp)
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
void	ft_skip_export_tokens(t_list **list)
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
 * @brief Searches for the next logical operator (AND/OR) in the token list.
 * 
 * This function iterates through the token list, looking for the first token
 * of type `AND` or `OR`. It returns `true` if such a token is found,indicating
 * that a logical operator has been encountered. If a token of type `PRTHESES`
 * (parentheses) is found, the search stops and the function returns `false`, 
 * indicating no logical operator is found before a parenthesis.
 * 
 * @param list A pointer to the token list, which will be updated as the function
 *        iterates through it.
 * @return `true` if an AND or OR operator is found, `false` otherwise.
 */
static bool ft_find_next_root(t_list **list) 
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == AND || token->type == OR)
			return (true);
		if (token->type == PRTHESES)
			break ;
		*list = (*list)->next;
	}
	return (false);
}

/**
 * @brief Builds a root node for a binary tree from the given token list.
 * 
 * The function processes the token list to create a root node of type `t_node`.
 * It handles logical operators (`AND`, `OR`) and recursively builds left and 
 * right branches based on the tokens. It skips export tokens and appropriately
 * links child nodes to the root.
 * 
 * @param list A pointer to the token list that will be parsed.
 * @param type The type of the root node (logical operator or other).
 * 
 * @return A pointer to the newly created root node, or NULL in case of
 *         allocation failure.
 */
void	*ft_build_root(t_list **list, t_type type)
{
	t_node	*root;

	if (ft_validate_skip(list))
		ft_skip_export_tokens(list);
	root = (t_node *)malloc(sizeof(t_node));
	if (!root)
		return (NULL); //ft_error_hanlder(); 1 // malloc failed
	root->left = NULL;
	root->right = NULL;
	root->parent_node = NULL;
	root->type = type;
	if (type == AND || type == OR)
		*list = (*list)->next;
	root->left = ft_build_tree(list, &root);
	if (!root->left)
		return ((void *)root);
	if (!list || !*list)
		return ((void *)root);
	if (ft_find_next_root(list))
	{
		type = ((t_token *)(*list)->content)->type;
		root->right = ft_build_root(list, type); //colocar parent_node?
	}
	return ((void *)root);
}
