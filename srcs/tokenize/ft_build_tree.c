#include "../../includes/minishell.h"

bool	ft_validate_next_token(t_list **list)
{
	//write brief
	if (((t_token *)(*list)->next->content)->type == PIPE)
		return (false);
	else if (((t_token *)(*list)->next->content)->type == AND)
		return (false);
	else if (((t_token *)(*list)->next->content)->type == OR)
		return (false);
	else
		return (true);
}

/**
 * @brief Searches for the next pipe token in the list and advances the pointer.
 * 
 * Iterates through the token list to find the next pipe ('|') token. If a pipe is found, 
 * the list pointer is updated to the token immediately after the pipe and returns true. 
 * Otherwise, the pointer reaches the end of the list and returns false.
 * 
 * @param list A double pointer to the token list, updated to the position after the pipe if found.
 * @return true if a pipe token is found, otherwise false.
 */
static bool	ft_find_next_pipe(t_list **list)
{
	//update brief
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
		{
			*list = (*list)->next;
			return (true);
		}
		if (token->type == AND || token->type == OR) // and PRTHESIS?
			return (false);
		*list = (*list)->next;
	}
	return (false);
}


/**
 * @brief Constructs a binary tree representing a pipeline structure.
 * 
 * Builds a binary tree where each node represents a pipe ('|') and its children 
 * represent the commands or branches connected by that pipe. The function recursively 
 * processes the token list, skipping invalid tokens and linking branches. 
 * 
 * @param list A double pointer to the token list, updated as tokens are consumed during tree building.
 * @return A pointer to the root of the constructed binary tree, or NULL if an error occurs.
 */
void	*ft_build_tree(t_list **list)
{
	t_pipe	*pipe;

	ft_skip_export_tokens(list);
	pipe = (t_pipe *)malloc(sizeof(t_pipe));
	if (!pipe)
		return (NULL); //ft_error_hanlder(); malloc failed
	pipe->left = NULL;
	pipe->right = NULL;
	pipe->type = PIPE;
	pipe->left = ft_build_branch(list, NULL);
	if (!pipe->left)
		return (NULL);
	if (!list || !*list)
		return ((void *)pipe);
	if (ft_find_next_pipe(list))
		pipe->right = ft_build_tree(list);
	return ((void *)pipe);
}
