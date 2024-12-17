/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_input_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:25:49 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/17 17:33:48 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Processes the input string to build an execution tree.
 * 
 * This function takes input string, trims leading and trailing whitespace,
 * validates its syntax, and tokenizes it into a list. It then constructs a
 * syntax tree representing the input commands and redirections. If any step
 * fails, the function cleans up allocated memory and returns NULL.
 * 
 * @param input The raw input string to process.
 * @return A pointer to the root of the syntax tree if successful, or NULL on
 *         failure.
 */
void	*ft_process_input(char *input)
{
	t_list	**token_list;
	char	*trimmed;
	void	*root;
	t_list	*head;

	trimmed = ft_strtrim(input, ISSPACE);
	if (!trimmed)
		return (ft_error_malloc("ft_strtrim"), NULL);
	if (!ft_validate_syntax(trimmed))
		return (free(trimmed), NULL);
	token_list = ft_create_token_list(trimmed);
	if (!token_list)
		return (free(trimmed), NULL);
	root = NULL;
	head = *token_list;
	if (token_list && *token_list)
		root = ft_build_root(token_list, ROOT);
	free(trimmed);
	ft_free_list(head);
	free(token_list);
	return (root);
}
