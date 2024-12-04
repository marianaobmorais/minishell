#include "../../includes/minishell.h"

/**
 * @brief Extracts executable arguments from a token list.
 * 
 * This function parses a list of tokens to extract arguments that are of 
 * executable type (`EXEC`). It skips redirection tokens (`REDIR`) and their
 * associated targets. The function halts processing if it encounters a node 
 * token (`NODE`) or a parentheses token (`PRTHESES`), as these indicate the 
 * end of the argument list.
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
		return (ft_error_malloc("args"), NULL);
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
		else if (ft_is_token_type(token, NODE) || token->type == PRTHESES)
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
	exec->pathname = token->value;
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

/**
 * @brief Searches for the next redirection token in the token list.
 * 
 * This function iterates through a token list, starting from the 
 * current position, to locate the next token of type `REDIR`. The search 
 * stops at tokens of type `NODE` or at the end of the list.
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
		if (ft_is_token_type(token, NODE))
			break ;
		if (ft_is_token_type(token, REDIR))
			return (true);
		*list = (*list)->next;
	}
	return (false);
}

/**
 * @brief Assigns mode and file descriptor values based on redirection type.
 * 
 * Configures the file opening mode (e.g., read, write, append) and the
 * standard file descriptor (e.g., stdin, stdout) for a redirection node based
 * on its type.
 * 
 * @param redir Double pointer to the redirection structure to configure.
 */
void	ft_assign_redir_mode(t_redir **redir)
{
	if ((*redir)->type == OUTFILE)
		(*redir)->mode = O_WRONLY | O_CREAT | O_TRUNC;
	else if ((*redir)->type == INFILE)
		(*redir)->mode = O_RDONLY;
	else if ((*redir)->type == APPEND)
		(*redir)->mode = O_WRONLY | O_CREAT | O_APPEND;
	else
		(*redir)->mode = -1;
}
/**
 * @brief Initializes a redirection structure based on the given token.
 * 
 * This function creates and initializes a `t_redir` structure,which represents
 * a redirection in the shell. It also sets up a linked list (`t_list **`) to 
 * store the target of the redirection. The type of redirection (e.g., input, 
 * output, append, etc.) is determined from the token, and the appropriate
 * redirection mode is assigned. If the next token is an executable, it is
 * considered the target of the redirection, and added to the target list.
 * 
 * @param token The token representing the redirection.
 * @param list The list of tokens to extract the redirection target from.
 * @return A pointer to the initialized `t_redir` structure, or `NULL` if
 *         memory allocation fails.
 */
t_redir	*ft_init_redir(t_token *token, t_list **list)
{
	t_redir	*redir;
	t_list	**target;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (ft_error_malloc("redir"), NULL);
	target = (t_list **)malloc(sizeof(t_list *));
	if (!target)
		return (ft_error_malloc("target"), NULL);
	*target = NULL;
	redir->target = NULL;
	redir->next = NULL;
	redir->type = token->type;
	ft_assign_redir_mode(&redir);
	if ((*list)->next && (*list)->next->content
		&& ((t_token *)(*list)->next->content)->type == EXEC)
	{
		*list = (*list)->next;
		ft_lstadd_back(target, ft_lstnew((t_token *)(*list)->content)); 
		redir->target = target;
	}
	return (redir);
}
