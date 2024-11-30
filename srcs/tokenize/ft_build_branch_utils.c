#include "../../includes/minishell.h"

/**
 * @brief Extracts executable arguments from a token list.
 * 
 * This function parses a list of tokens to extract arguments that are of 
 * executable type (`EXEC`). It skips redirection tokens (`REDIR`) and their
 * associated targets. The function halts processing if it encounters a `PIPE`
 * or a `NULL` as these indicate the end of the argument list.
 * 
 * The extracted arguments are stored in a new list (`t_list **`), which is 
 * dynamically allocated and returned to the caller.
 * 
 * @param list The list of tokens to extract arguments from.
 * @return A pointer to a new list (`t_list **`) containing the arguments, 
 *         or `NULL` if memory allocation fails.
 */
t_list	**ft_get_args(t_list **list)
{
	t_list	**args;
	t_list	*curr;
	t_token	*token;

	args = (t_list **)malloc(sizeof(t_list *));
	if (!args)
		return (NULL); // ft_error_handler(); 1 // malloc error
	*args = NULL;
	curr = *list;
	while (curr)
	{
		token = (t_token *)curr->content;
		if (ft_is_token_type(token, EXEC))
			ft_lstadd_back(args, ft_lstnew(curr->content));
		else if (ft_is_token_type(token, REDIR))
		{
			if (curr->next && ((t_token *)curr->next->content)->type != PIPE)
				curr = curr->next;
		}
		else if (token->type == PIPE)
			break ;
		curr = curr->next;
	}
	return (args);
}

/**
 * @brief Searches for the next redirection token in the token list.
 * 
 * This function iterates through a token list, starting from the 
 * current position, to locate the next token of type `REDIR`. The search 
 * stops at tokens of type `PIPE` or at the end of the list.
 * 
 * If a redirection token is found, the function returns `true`, and the 
 * pointer to the list is updated to the position of the found token. 
 * Otherwise, it returns `false`.
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
		if (token->type == PIPE)
			break ;
		if (ft_is_token_type(token, REDIR))
			return (true);
		*list = (*list)->next;
	}
	return (false);
}
