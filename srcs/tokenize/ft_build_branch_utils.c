#include "../../includes/minishell.h"

/**
 * @brief Creates a sub-list of tokens enclosed within parentheses.
 * 
 * This function extracts a sub-list of tokens from a token list, starting 
 * from the current position and including all tokens up to, but not 
 * including, the closing parenthesis `)` that matches the opening 
 * parenthesis `(` before the current position.
 * 
 * The function maintains a nesting count (`count`) to handle nested 
 * parentheses correctly, ensuring that the correct closing parenthesis 
 * is matched.
 * 
 * The extracted tokens are stored in a new list (`t_list **`), which is 
 * dynamically allocated and returned to the caller.
 * 
 * @param list A pointer to the list of tokens. This pointer will be updated 
 *        to point to the token immediately following the closing parenthesis.
 * 
 * @return A pointer to a new list (`t_list **`) containing the sub-list 
 *         of tokens, or `NULL` if memory allocation fails.
 */

t_list	**ft_create_sub_list(t_list **list)
{
	t_list	**sub;
	t_token	*token;
	int		count;

	sub = (t_list **)malloc(sizeof(t_list *));
	if (!sub)
		return (NULL); //error_handler; 1 //malloc failed
	*sub = NULL;
	count = 1;
	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PRTHESES && token->value[0] == '(')
			count++;
		if (token->type == PRTHESES && token->value[0] == ')')
			count--;
		if (count == 0)
		{
			*list = (*list)->next;
			break;
		}
		ft_lstadd_back(sub, ft_lstnew(token));
		*list = (*list)->next;
	}
	return (sub);
}
/**
 * @brief Creates a subroot node for a subtree from a sub-list of tokens.
 * 
 * This function processes a subexpression enclosed in parentheses, 
 * extracting the tokens between the parentheses, and constructing a 
 * subtree for those tokens. It skips the opening parenthesis `(` and 
 * processes the tokens up to, but not including, the matching closing 
 * parenthesis `)`.
 * 
 * The tokens within the parentheses are extracted into a sub-list using 
 * `ft_create_sub_list`, which is then passed to `ft_build_root` to build 
 * a subtree. The created subtree is encapsulated in a subroot node of 
 * type `t_node`.
 * 
 * @param list A pointer to the current token list. This pointer will 
 *        be updated to point to the token immediately following the 
 *        closing parenthesis `)`.
 * 
 * @return A pointer to the newly created `t_node` representing the 
 *         root of the subtree, or `NULL` if memory allocation fails 
 *         or the sub-list is invalid.
 */

t_node	*ft_create_subroot_node(t_list **list)
{
	t_node	*sub_root; 
	t_list	**sub_list;

	sub_root = NULL;
	*list = (*list)->next;
	sub_list = ft_create_sub_list(list);
	if (!sub_list || !*sub_list)
		return (NULL); // error_handler; 1 //malloc failed
	printf("sub list:\n"); //debug
	ft_print_list(sub_list); //debug
	sub_root = ft_build_root(sub_list, SUB_ROOT);
	ft_free_list(sub_list);
	return (sub_root);
}

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
 * 
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
		else if (ft_is_token_type(token, NODE) || token->type == PRTHESES)
			break ;
		curr = curr->next;
	}
	return (args);
}

/**
 * @brief Creates an execution node based on the current token.
 * 
 * Allocates memory for an execution node, assigns its type, pathname, and arguments, 
 * and processes subsequent tokens in the list until a pipe, redirection, or NULL is encountered.
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
		return (NULL); //ft_error_hanlder(); 1 //malloc failed
	exec->type = token->type;
	exec->pathname = token->value;
	exec->args = ft_get_args(list);
	if (!exec->args)
		return (NULL); //ft_error_hanlder(); 1 //malloc failed
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
 * 
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
	else if ((*redir)->type == HEREDOC)
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
 * 
 * @return A pointer to the initialized `t_redir` structure, or `NULL` if
 *         memory allocation fails.
 */

t_redir	*ft_init_redir(t_token *token, t_list **list)
{
	t_redir	*redir;
	t_list	**target;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL); //ft_error_hanlder(); 1 // malloc failed
	target = (t_list **)malloc(sizeof(t_list *));
	if (!target)
		return (NULL); // ft_error_handler(); 1 // malloc failed
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
