#include "../../includes/minishell.h"

/**
 * @brief Assigns mode and file descriptor values based on redirection type.
 * 
 * Configures the file opening mode (e.g., read, write, append) and the standard 
 * file descriptor (e.g., stdin, stdout) for a redirection node based on its type.
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
 * @brief Initializes a redirection node based on the current token.
 * 
 * Allocates memory for a redirection node, assigns its type, mode, and file 
 * descriptor, and sets the target based on the next token in the list.
 * 
 * @param token The current token representing the redirection type.
 * @param list Pointer to the token list, updated to point to the target node.
 * @return A pointer to the initialized redirection node, or NULL on failure.
 */
static t_redir	*ft_init_redir(t_token *token, t_list **list)
{
	t_redir	*redir; //update brief
	t_list	**target;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL); //ft_error_hanlder(); 1 // malloc failed
	target = (t_list **)malloc(sizeof(t_list *)); //create t_list ** 
	if (!target)
		return (NULL); // ft_error_handler(); 1 // malloc failed
	*target = NULL;
	redir->target = NULL;
	redir->next = NULL;
	redir->type = token->type;
	ft_assign_redir_mode(&redir);
	if ((*list)->next && (*list)->next->content && ((t_token *)(*list)->next->content)->type == EXEC)
	{
		*list = (*list)->next; // move up to target
		ft_lstadd_back(target, ft_lstnew((t_token *)(*list)->content)); 
		redir->target = target;
	}
	return (redir);
}

/**
 * @brief Creates a redirection node and links it to subsequent nodes.
 * 
 * Constructs a redirection node for the specified token and links it to the next 
 * relevant node (either another redirection, an execution node, or NULL). Updates 
 * the token list pointer as nodes are processed.
 * 
 * @param token The current token representing the redirection type.
 * @param list Pointer to the token list, updated during processing.
 * @param exec Pointer to the execution node, if available, for linking.
 * @return A pointer to the created redirection node, or NULL on failure.
 */
static t_redir	*ft_create_redir_node(t_token *token, t_list **list, t_exec *exec, t_node *sub_root)
{
	//update brief
	t_redir	*redir;
	t_list	*list_next;

	redir = ft_init_redir(token, list);
	if (!redir)
		return (NULL); //ft_error_hanlder(); 1 //malloc failed
	if (!(*list)->next || ft_is_token_type(((t_token *)(*list)->next->content), NODE))
		redir->next = (void *)exec; //if next is NULL or PIPE or AND or OR (NOT PRNTHESES)
	else
	{
		*list = (*list)->next; // move up to next node
		list_next = *list;

		if (sub_root)
			redir->next = (void *)sub_root;

		else if (!exec && ((t_token *)(*list)->content)->type == EXEC)
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
		*list = (*list)->next; //advances in list memory
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
	t_token	*token; //update brief
	t_redir	*redir;
	t_node	*sub_root;

	sub_root = NULL;
	token = (*list)->content;
	if (!exec && (ft_is_token_type(token, EXEC)))
	{
		exec = ft_create_exec_node(token, list); //advances token_list
		if (!list || !*list || !exec)
			return ((void *)exec);
		token = (*list)->content;
		if (ft_is_token_type(token, NODE) || token->type == PRTHESES)
			return ((void *)exec);
	}
	if (token->type == PRTHESES && token->value[0] == '(')
	{
		sub_root = ft_create_subroot_node(list); //advanced in token list
		//need to do checks of list and malloc here
		token = (*list)->content;
		if (!ft_is_token_type(token, REDIR))
			return ((void *)sub_root);
	}
	if (ft_is_token_type(token, REDIR))
	{
		redir = ft_create_redir_node(token, list, exec, sub_root);
		return ((void *)redir);
	}
	return (NULL); //error in creating branch; 1
}
