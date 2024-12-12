#include "../../includes/minishell_bonus.h"

/**
 * @brief Handles the linking of the next node in the redirection chain.
 * 
 * This function determines and sets the `next` field for a redirection
 * (`REDIR`) node. It advances the token list and examines the type of the
 * subsequent token to decide whether to attach an execution (`EXEC`) node, a
 * subtree (`SUB_ROOT`), or another redirection. If a redirection is
 * encountered before any pipe or other logical operator, it recursively
 * processes the redirection chain.
 * 
 * @param list A double pointer to the current position in the token list.
 *        The function advances this pointer to the next token.
 * @param redir A pointer to the current `REDIR` node being processed.
 * @param exec A pointer to the `EXEC` node, if present, for execution context.
 * @param sub_r A pointer to the `SUB_ROOT` node, if present, for subtree
 *        context.
 * @return A pointer to the updated `REDIR` node with the `next` field
 *         correctly linked.
 */
static void	*ft_handle_next_node(t_list **list, t_redir *redir, \
	t_exec *exec, t_node *sub_root)
{
	t_list	*list_next;

	*list = (*list)->next;
	list_next = *list;
	if (!exec && ((t_token *)(*list)->content)->type == EXEC)
		redir->next = ft_build_branch(&list_next, exec, sub_root);
	else if (ft_find_next_redir(&list_next))
		redir->next = ft_build_branch(&list_next, exec, sub_root);
	else
	{
		if (sub_root)
			redir->next = (void *)sub_root;
		redir->next = (void *)exec;
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
	t_exec *exec, t_node *sub_root)
{
	t_redir	*redir;

	redir = ft_init_redir(token, list);
	if (!redir)
		return (NULL);
	if (!(*list)->next
		|| ft_is_token_type(((t_token *)(*list)->next->content), NODE))
	{
		if (sub_root)
			redir->next = (void *)sub_root;
		else
			redir->next = (void *)exec;
	}
	else
		redir = ft_handle_next_node(list, redir, exec, sub_root);
	return (redir);
}

/**
 * @brief Handles the creation of an execution node in the syntax tree.
 * 
 * This function creates an `EXEC` node for a given token and advances the 
 * token list. If the token type is not compatible with further node creation 
 * (e.g., if it is a `NODE` or a `PRTHESES`), the function returns the created 
 * `EXEC` node. Otherwise, it returns `NULL` for further processing.
 * 
 * @param list A double pointer to the current position in the token list. 
 *        The function advances the list pointer if an `EXEC` node is created.
 * @param exec A double pointer to the `t_exec` structure where the newly 
 *        created `EXEC` node is stored.
 * @param token A double pointer to the current token being processed. The 
 *        function updates this pointer to the next token in the list.
 * @return A pointer to the created `EXEC` node, or `NULL` if further processing
 *         of the token list is required.
 */
static void	*ft_handle_exec_node(t_list **list, t_exec **exec, t_token **token)
{
	*exec = ft_create_exec_node(*token, list);
	if (!list || !*list || !*exec)
		return ((void *)*exec);
	*token = (*list)->content;
	if (ft_is_token_type(*token, NODE) || (*token)->type == PRTHESES)
		return ((void *)*exec);
	return (NULL);
}

/**
 * @brief Handles the creation of a subroot node in the syntax tree.
 * 
 * This function creates a subtree (subroot node) for a parenthesized 
 * expression in the token list. It processes the tokens within parentheses 
 * and advances the list pointer. If the next token after the parentheses 
 * does not represent a redirection, the function returns the created subroot 
 * node. Otherwise, it returns `NULL` for further processing.
 * 
 * @param list A double pointer to the current position in the token list. 
 *        The function advances the list pointer as it processes the 
 *        parenthesized tokens.
 * @param sub_r A double pointer to the `t_node` structure where the newly 
 *        created subroot node is stored.
 * @param token A double pointer to the current token being processed. The 
 *        function updates this pointer to the next token in the list.
 * @return A pointer to the created subroot node, or `NULL` if further
 *         processing of the token list is required.
 */
static void	*ft_handle_subroot_node(t_list **list, t_node **sub_r, \
	t_token **token)
{
	*sub_r = ft_create_subroot_node(list);
	if (!list || !*list || !*sub_r)
		return ((void *)*sub_r);
	*token = (*list)->content;
	if (!ft_is_token_type(*token, REDIR))
		return ((void *)*sub_r);
	return (NULL);
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
void	*ft_build_branch(t_list **list, t_exec *exec, t_node *sub_root)
{
	t_token	*token;
	void	*result;

	token = (*list)->content;
	if (!exec && (ft_is_token_type(token, EXEC)))
	{
		result = ft_handle_exec_node(list, &exec, &token);
		if (result)
			return (result);
	}
	if (token->type == PRTHESES && token->value[0] == '(')
	{
		result = ft_handle_subroot_node(list, &sub_root, &token);
		if (result)
			return (result);
	}
	if (ft_is_token_type(token, REDIR))
	{
		result = ft_create_redir_node(token, list, exec, sub_root);
		return ((void *)result);
	}
	return (NULL);
}
