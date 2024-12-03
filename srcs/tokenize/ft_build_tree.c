#include "../../includes/minishell.h"

static bool	ft_validate_skip(t_list **list)
{
	//write brief
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
	if (ft_is_token_type(((t_token *)tmp->content), PIPE)
		|| !ft_is_token_type(((t_token *)tmp->content), EXEC))
		return (false);
	return (true);
}

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
	t_token	*token;

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
				|| ((t_token *)(*list)->next->content)->type == PIPE)
			break ;
		else
		{
			*list = (*list)->next;
			token = (*list)->content;
		}
	}
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
void	*ft_build_tree(t_list **list)
{
	t_pipe	*pipe;

	if (ft_validate_skip(list))
		ft_skip_export_tokens(list);
	pipe = (t_pipe *)malloc(sizeof(t_pipe));
	if (!pipe)
		return (NULL); //ft_error_hanlder(); 1// malloc failed
	pipe->type = PIPE;
	pipe->right = NULL; //already here
	pipe->left = NULL;
	pipe->left = ft_build_branch(list, NULL);
	if (!pipe->left)
		return (NULL);
	if (!list || !*list)
		return ((void *)pipe);
	if (ft_find_next_pipe(list))
		pipe->right = ft_build_tree(list);
	else
		pipe->right = NULL; // redundant?
	return ((void *)pipe);
}
