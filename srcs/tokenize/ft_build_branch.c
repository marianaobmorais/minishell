#include "../../includes/minishell.h"

/**
 * @brief Assigns mode and file descriptor values based on redirection type.
 * 
 * Configures the file opening mode (e.g., read, write, append) and the
 * standard file descriptor (e.g., stdin, stdout) for a redirection node based
 * on its type.
 * 
 * @param redir Double pointer to the redirection structure to configure.
 */
static void	ft_assign_redir_mode(t_redir **redir)
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
 * @return A pointer to the initialized `t_redir` structure, or `NULL` if
 *         memory allocation fails.
 */
static t_redir	*ft_init_redir(t_token *token, t_list **list)
{
	t_redir	*redir;
	t_list	**target;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL); //ft_error_hanlder(); 1// malloc failed
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
		*list = (*list)->next; // move up to target
		ft_lstadd_back(target, ft_lstnew((t_token *)(*list)->content));
		redir->target = target;
	}
	return (redir);
}

/**
 * @brief Creates a redirection (`REDIR`) node and links its subsequent node.
 * 
 * This function initializes a `REDIR` node for a redirection token and
 * determines its `next` field. It handles scenarios where the redirection is
 * followed by an execution (`EXEC`) node, or another redirection. If no
 * further nodes are present, it appropriately sets the next` field to `NULL`
 * or to the provided `EXEC` node.
 * 
 * @param token A pointer to the current token being processed for redirection.
 * @param list A double pointer to the token list. The function advances
 *        the list pointer as it processes subsequent tokens.
 * @param exec A pointer to the `EXEC` node, if present, for execution context.
 * @return A pointer to the newly created `REDIR` node, or `NULL` if an error
 *         occurs.
 */
static t_redir	*ft_create_redir_node(t_token *token, t_list **list, t_exec *exec)
{
	t_redir	*redir;
	t_list	*list_next;

	redir = ft_init_redir(token, list);
	if (!redir)
		return (NULL);
	if (!(*list)->next || ((t_token *)(*list)->next->content == PIPE)) //check whether next is not NULL or PIPE
		redir->next =  (void *)exec;
	else
	{
		*list = (*list)->next; // move up to next node
		list_next = *list;
		if (!exec && ((t_token *)(*list)->content)->type == EXEC)
			redir->next = ft_build_branch(&list_next, exec);
		else if (ft_find_next_redir(&list_next)) // update pointer to next redir before pipe
			redir->next = ft_build_branch(&list_next, exec);
		else
			redir->next = (void *)exec; //whether it's is NULL or not
	}
	return (redir);
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
static t_exec	*ft_create_exec_node(t_token *token, t_list **list)
{
	t_exec	*exec;

	exec = (t_exec *)malloc(sizeof(t_exec));
	if (!exec)
		return (NULL); //ft_error_hanlder(); 1// malloc failed
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
 * @brief Constructs a branch in the execution tree.
 * 
 * Processes tokens to create either an execution node or a redirection node, linking them 
 * to form a branch in the execution tree. Handles various token types such as commands, 
 * redirections, and pipes.
 * 
 * @param list Pointer to the token list, updated during processing.
 * @param exec Pointer to an existing execution node, if available, for linking.
 * @return A pointer to the created branch node (either execution or redirection), or NULL on failure.
 */
void	*ft_build_branch(t_list **list, t_exec *exec)
{
	t_token	*token;
	t_redir	*redir;

	token = (*list)->content;
	if (!exec && (ft_is_token_type(token, EXEC)))
	{
		exec = ft_create_exec_node(token, list);
		if (!list || !*list || !exec)
			return ((void *)exec);
		token = (*list)->content;
		if (token->type == PIPE)
			return ((void *)exec);
	}
	if (ft_is_token_type(token, REDIR))
	{
		redir = ft_create_redir_node(token, list, exec);
		return ((void *)redir);
	}
	return (NULL);
}
