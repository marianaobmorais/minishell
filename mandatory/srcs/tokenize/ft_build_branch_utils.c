/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_build_branch_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:32:00 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 17:32:01 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Extracts arguments from the token list and stores them in a new list.
 *
 * This function iterates through the provided token list, identifying tokens of
 * type `EXEC` and adding their values to a newly allocated argument list. It
 * skips over `REDIR` tokens and their associated values, as well as stopping
 * when a `NODE` token is encountered. The resulting list contains arguments in
 * the order they were found.
 *
 * @param list A double pointer to the token list from which arguments are
 *        extracted.
 * @return A double pointer to the newly created argument list, or `NULL` if
 *         memory allocation fails.
 */
static t_list	**ft_get_args(t_list **list)
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
