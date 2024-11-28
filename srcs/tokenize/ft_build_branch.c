#include "../../includes/minishell.h"

static void	*ft_handle_next_node(t_list **list, t_redir *redir, \
	t_exec *exec, t_node *sub_r)
{
	t_list	*list_next;

	*list = (*list)->next; // move up to next node
	list_next = *list;
	if (!exec && ((t_token *)(*list)->content)->type == EXEC)
		redir->next = ft_build_branch(&list_next, exec, sub_r);
	else if (ft_find_next_redir(&list_next)) // update pointer to next redir before pipe
		redir->next = ft_build_branch(&list_next, exec, sub_r);
	else
	{
		if (sub_r)
			redir->next = (void *)sub_r;
		redir->next = (void *)exec; //whether it's is NULL or not
	}
	return (redir);
}

static t_redir	*ft_create_redir_node(t_token *token, t_list **list, \
	t_exec *exec, t_node *sub_root)
{
	//update brief
	t_redir	*redir;

	redir = ft_init_redir(token, list);
	if (!redir)
		return (NULL); //ft_error_hanlder(); 1 //malloc failed
	if (!(*list)->next || ft_is_token_type(((t_token *)(*list)->next->content), NODE))
	{
		if (sub_root)
			redir->next = (void *)sub_root;
		else
			redir->next = (void *)exec; //if next is NULL or PIPE or AND or OR (NOT PRNTHESES)
	}
	else
		redir = ft_handle_next_node(list, redir, exec, sub_root);
	return (redir);
}

/**
 * @brief Creates a redirection node and links it to subsequent nodes.
 * 
 * Constructs a redirection node for the specified token and links it to the
 * next relevant node (either another redirection, an execution node, or NULL).
 * Updates the token list pointer as nodes are processed.
 * 
 * @param token The current token representing the redirection type.
 * @param list Pointer to the token list, updated during processing.
 * @param exec Pointer to the execution node, if available, for linking.
 * @return A pointer to the created redirection node, or NULL on failure.
 */
// static t_redir	*ft_create_redir_node(t_token *token, t_list **list, t_exec *exec, t_node *sub_root)
// {
// 	//update brief
// 	t_redir	*redir;
// 	t_list	*list_next;

// 	redir = ft_init_redir(token, list);
// 	if (!redir)
// 		return (NULL); //ft_error_hanlder(); 1 //malloc failed
// 	if (!(*list)->next || ft_is_token_type(((t_token *)(*list)->next->content), NODE))
// 	{
// 		if (sub_root)
// 			redir->next = (void *)sub_root;
// 		else
// 			redir->next = (void *)exec; //if next is NULL or PIPE or AND or OR (NOT PRNTHESES)
// 	}
// 	else
// 	{
// 		*list = (*list)->next; // move up to next node
// 		list_next = *list;
// 		if (!exec && ((t_token *)(*list)->content)->type == EXEC)
// 			redir->next = ft_build_branch(&list_next, exec, sub_root);
// 		else if (ft_find_next_redir(&list_next)) // update pointer to next redir before pipe
// 			redir->next = ft_build_branch(&list_next, exec, sub_root);
// 		else
// 		{
// 			if (sub_root)
// 				redir->next = (void *)sub_root;
// 			redir->next = (void *)exec; //whether it's is NULL or not
// 		}
// 	}
// 	return (redir);
// }

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
 * 
 * @return A pointer to the created `EXEC` node, or `NULL` if further processing 
 *         of the token list is required.
 */
void	*ft_handle_exec_node(t_list **list, t_exec **exec, t_token **token)
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
 * 
 * @return A pointer to the created subroot node, or `NULL` if further processing 
 *         of the token list is required.
 */
void	*ft_handle_subroot_node(t_list **list, t_node **sub_r, t_token **token)
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
 * token types, including executable commands (`EXEC`), parentheses (`PRTHESES`), 
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
 * 
 * @return A pointer to the created branch node (`t_exec`, `t_node`, or `t_redir`) 
 *         or `NULL` if no branch could be built.
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
