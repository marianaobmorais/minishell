#include "../../includes/minishell.h"

/**
 * @brief Searches for the next redirection token in the token list.
 * 
 * This function iterates through a token list, starting from the current
 * position, to locate the next token of type `REDIR`. The search stops at
 * tokens of type `NODE` or at the end of the list. If a redirection token is
 * found, the function returns `true`, and the pointer to the list is updated to
 * the position of the found token. Otherwise, it returns `false`.
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
static void	ft_assign_redir_mode(t_redir **redir)
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
 * This function creates and initializes a `t_redir` structure, which represents
 * a redirection in the shell. It also sets up a linked list (`t_list **`) to 
 * store the target of the redirection. The type of redirection (e.g., input, 
 * output, append, heredoc) is determined from the token, and the appropriate
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
		ft_add_to_token_list(&((t_token *)(*list)->content)->value, target);
		redir->target = target;
		if (redir->type == HEREDOC)
			((t_token *)((*redir->target)->content))->expand = false;
	}
	return (redir);
}

/**
 * @brief Creates a redirection (`REDIR`) node and links its subsequent node.
 * 
 * This function initializes a `REDIR` node for a redirection token and
 * determines its `next` field. It handles scenarios where the redirection is
 * followed by an execution (`EXEC`) node, a subtree (`SUB_ROOT`), or another
 * redirection. If no further nodes are present, it appropriately sets the
 * `next` field to `NULL` or to the provided `EXEC` or `SUB_ROOT` node.
 * 
 * @param token A pointer to the current token being processed for redirection.
 * @param list A double pointer to the token list. The function advances
 *        the list pointer as it processes subsequent tokens.
 * @param exec A pointer to the `EXEC` node, if present, for execution context.
 * @param sub_root A pointer to the `SUB_ROOT` node, if present, for subtree
 *        context.
 * @return A pointer to the newly created `REDIR` node, or `NULL` if an error
 *         occurs.
 */
static t_redir	*ft_create_redir_node(t_token *token, t_list **list, \
	t_exec *exec)
{
	t_redir	*redir;
	t_list	*list_next;

	redir = ft_init_redir(token, list);
	if (!redir)
		return (NULL);
	if (!(*list)->next
		|| ft_is_token_type(((t_token *)(*list)->next->content), NODE))
			redir->next = (void *)exec;
	else
	{
		*list = (*list)->next;
		list_next = *list;
		if (!exec && ((t_token *)(*list)->content)->type == EXEC)
			redir->next = ft_build_branch(&list_next, exec);
		else if (ft_find_next_redir(&list_next))
			redir->next = ft_build_branch(&list_next, exec);
		else
			redir->next = (void *)exec;
	}
	return (redir);
}

/**
 * @brief Builds a branch of the syntax tree based on the current token type.
 * 
 * This function processes the current token in the token list and determines 
 * the appropriate branch to build for the syntax tree. It handles different 
 * token types,including executable commands (`EXEC`),parentheses (`PRTHESES`),
 * and redirections (`REDIR`). The function updates the corresponding structures
 * (execution nodes, subroot nodes, or redirection nodes) and advances the list 
 * pointer as needed.
 * 
 * @param list A double pointer to the current position in the token list. 
 *        The function processes the token at the current position.
 * @param exec A pointer to an `t_exec` structure representing an executable 
 *        command node. It is updated if an executable node is created.
 * @param sub_root A pointer to a `t_node` structure representing a subroot 
 *        (subtree for parenthesized expressions). It is updated if a subroot 
 *        node is created.
 * @return A pointer to the created branch node (`t_exec`, `t_node`, or
 *         `t_redir`) or `NULL` if no branch could be built.
 */
void	*ft_build_branch(t_list **list, t_exec *exec)
{
	t_token	*token; //update brief
	void	*result;

	token = (*list)->content;
	if (!exec && (ft_is_token_type(token, EXEC)))
	{
		exec = ft_create_exec_node(token, list);
		if (!list || !*list || !exec)
			return ((void *)exec);
		token = (*list)->content;
		if (ft_is_token_type(token, NODE))
			return ((void *)exec);
	}
	if (ft_is_token_type(token, REDIR))
	{
		result = ft_create_redir_node(token, list, exec);
		return ((void *)result);
	}
	return (NULL);
}
