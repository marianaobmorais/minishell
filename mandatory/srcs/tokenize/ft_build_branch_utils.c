#include "../../includes/minishell.h"

/**
 * @brief Extracts executable arguments from a token list.
 * 
 * This function parses a list of tokens to extract arguments that are of 
 * executable type (`EXEC`). It skips redirection tokens (`REDIR`) and their
 * associated targets. The function halts processing if it encounters a node 
 * token (`NODE`) or a parentheses token (`PRTHESES`), as these indicate the 
 * end of the argument list.
 * The extracted arguments are stored in a new list (`t_list **`), which is 
 * dynamically allocated and returned to the caller.
 * 
 * @param list The list of tokens to extract arguments from.
 * @return A pointer to a new list (`t_list **`) containing the arguments, 
 *         or `NULL` if memory allocation fails.
 */
static t_list	**ft_get_args(t_list **list)
{
	t_list	**args; //update brief
	t_list	*curr;
	t_token	*token;

	args = (t_list **)malloc(sizeof(t_list *));
	if (!args)
		return (ft_error_malloc("args"), NULL);
	*args = NULL;
	curr = *list;
	while (curr)
	{
		token = (t_token *)curr->content;
		if (ft_is_token_type(token, EXEC))
			ft_add_to_token_list(&token->value, args);
		else if (ft_is_token_type(token, REDIR))
		{
			if (curr->next && ((t_token *)curr->next->content)->type != PIPE)
				curr = curr->next;
		}
		else if (ft_is_token_type(token, NODE))
			break ;
		curr = curr->next;
	}
	return (args);
}

/**
 * @brief Creates an execution node based on the current token.
 * 
 * Allocates memory for an execution node, assigns its type, pathname, and
 * arguments, and processes subsequent tokens in the list until a pipe,
 * redirection, or NULL is encountered.
 * 
 * @param token The current token representing the execution command.
 * @param list Pointer to the token list, updated as nodes are processed.
 * @return A pointer to the created execution node, or NULL on failure.
 */
t_exec	*ft_create_exec_node(t_token *token, t_list **list)
{
	t_exec	*exec;

	exec = (t_exec *)malloc(sizeof(t_exec));
	if (!exec)
		return (ft_error_malloc("exec"), NULL);
	exec->type = token->type;
	exec->args = ft_get_args(list);
	if (!exec->args)
		return (ft_error_malloc("exec->args"), NULL);
	token = (*list)->content;
	while (*list && (ft_is_token_type(token, EXEC)))
	{
		token = (t_token *)(*list)->content;
		if (!ft_is_token_type(token, EXEC))
			break ;
		*list = (*list)->next;
	}
	return (exec);
}
