#include "../../includes/minishell.h"

t_list	**ft_create_sub_list(t_list **list)
{
	t_list	**sub; //write brief
	t_token	*token;
	int		count;

	sub = (t_list **)malloc(sizeof(t_list *));
	if (!sub)
		return (NULL); //error malooc
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
			*list = (*list)->next; //does not include last ')' to sub
			break;
		}
		ft_lstadd_back(sub, ft_lstnew(token));
		*list = (*list)->next; //advances in list memory
	}
	return (sub);
}

t_node	*ft_create_subroot_node(t_list **list)
{
	t_node	*sub_root; //write brief
	t_list	**sub_list;

	sub_root = NULL;
	*list = (*list)->next; //skip '('
	sub_list = ft_create_sub_list(list);
	if (!sub_list || !*sub_list)
		return (NULL); // Error malloc //but also empty sublist?? empty sublist is ruled out in syntax validation
	printf("sub list:\n"); //debug
	ft_print_list(sub_list); //debug
	sub_root = ft_build_root(sub_list, SUB_ROOT); // Process sublist into a subtree
	ft_free_list(sub_list);
	return (sub_root);
}

/**
 * @brief Extracts a list of argument strings from a token list.
 * 
 * Iterates through a token list, collecting values of tokens with types `EXEC`, `EXPORT`, 
 * or `EXPORT_AP` into a dynamically allocated string vector. Skips over redirection tokens 
 * and stops at a pipe (`PIPE`) token or the end of the list.
 * 
 * @param list A double pointer to the token list.
 * @return A dynamically allocated string vector containing the extracted arguments, 
 *         or NULL if no arguments are found.
 */
t_list	**ft_get_args(t_list **list)
{
	//update brief
	t_list	**args;
	t_list	*curr;
	t_token	*token;

	args = (t_list **)malloc(sizeof(t_list *)); //create t_list **
	if (!args)
		return (NULL); // ft_error_handler();
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
 * @brief Searches for the next redirection token in the token list.
 * 
 * Scans the token list to find the next redirection token (`OUTFILE`, `INFILE`, `APPEND`, 
 * or `HEREDOC`). Stops scanning at a pipe (`PIPE`) token or the end of the list.
 * 
 * @param list A double pointer to the token list, updated as tokens are consumed during the search.
 * @return true if a redirection token is found, otherwise false.
 */
bool	ft_find_next_redir(t_list **list)
{
	//update brief
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (ft_is_token_type(token, NODE) /* || token->type == PRTHESES */)
			break ;
		if (ft_is_token_type(token, REDIR))
			return (true);
		*list = (*list)->next;
	}
	return (false);
}

/**
 * @brief Searches for the next executable token in the token list.
 * 
 * Scans the token list to find the next executable token (`EXEC`). Stops scanning 
 * at a pipe (`PIPE`) token or the end of the list.
 * 
 * @param list A double pointer to the token list, updated as tokens are consumed during the search.
 * @return true if an executable token is found, otherwise false.
 */
bool	ft_find_next_exec(t_list **list)
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (ft_is_token_type(token, NODE) || token->type == PRTHESES) //include PRTHESES check
			break ;
		if (token->type == EXEC)
			return (true);
		*list = (*list)->next;
	}
	return (false);
}
