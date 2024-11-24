#include "../../includes/minishell.h"

/**
 * @brief Skips consecutive `EXPORT` tokens in the list.
 * 
 * Advances the token list pointer past consecutive `EXPORT` or `EXPORT_AP` tokens. 
 * Stops if a pipe ('|') token or the end of the list is reached.
 * 
 * @param list A double pointer to the token list, updated to the first non-`EXPORT` token 
 *             or a pipe token.
 */
void	ft_skip_export_tokens(t_list **list)
{
	//update brief
	t_token	*token;

	token = (*list)->content;
	while (*list && (token->type == EXPORT || token->type == EXPORT_AP))
	{
		if (!((*list)->next) || !ft_validate_next_token(list))
			break ;
		else
		{
			*list = (*list)->next;
			token = (*list)->content;
		}
	}
}

static bool ft_find_next_root(t_list **list) 
{
	t_token	*token;

	while (*list) // need to skip the logic operators inside parentheses
	{
		token = (t_token *)(*list)->content;
		if (token->type == AND || token->type == OR)
			return (true);
		if (token->type == PRTHESES) //included PRTHESES check
			break ;
		*list = (*list)->next; //this function changes memory
	}
	return (false);
}

// void	*ft_build_root(t_list **list, t_type type)
// {
// 	t_node	*root; //write brief

// 	ft_skip_export_tokens(list);
// 	root = (t_node *)malloc(sizeof(t_node));
// 	if (!root)
// 		return (NULL); //ft_error_hanlder(); malloc failed
// 	root->left = NULL;
// 	root->right = NULL;
// 	root->parent_node = NULL;
// 	root->type = type;
// 	if (type != ROOT) // move up to next node in token list
// 		*list = (*list)->next;
// 	root->left = ft_build_tree(list, &root);
// 	if (!root->left)
// 		return (NULL);
// 	if (!list || !*list)
// 		return ((void *)root);
// 	if (ft_find_next_root(list))
// 	{
// 		type = ((t_token *)(*list)->content)->type;
// 		root->right = ft_build_root(list, type);
// 	}
// 	return ((void *)root);
// }

void	*ft_build_root(t_list **list, t_type type)
{
	t_node	*root; //write brief

	ft_skip_export_tokens(list);
	root = (t_node *)malloc(sizeof(t_node));
	if (!root)
		return (NULL); //ft_error_hanlder(); malloc failed
	//root = NULL; //why this gives me seg fault?
	root->left = NULL;
	root->right = NULL;
	root->parent_node = NULL;
	root->type = type;
	if (type == AND || type == OR) // move up to next node in token list. skip && or ||
		*list = (*list)->next;
	root->left = ft_build_tree(list, &root);
	if (!root->left)
		return (NULL);
	if (!list || !*list)
		return ((void *)root);
	if (ft_find_next_root(list)) // need to skip logic operators inside parentheses
	{
		type = ((t_token *)(*list)->content)->type;
		root->right = ft_build_root(list, type); //colocar parent_node?
	}
	return ((void *)root);
}
