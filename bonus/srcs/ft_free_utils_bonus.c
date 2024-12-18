/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:49:17 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/18 10:20:25 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_bonus.h"

/**
 * @brief Frees all strings in a vector (array of strings) and the vector
 *        itself.
 *
 * This function iterates through the array of strings (vector), freeing each
 * string individually. After all strings are freed, the function frees the
 * vector pointer itself to prevent memory leaks.
 *
 * @param vector A pointer to the array of strings to be freed.
 */
void	ft_free_vector(char **vector)
{
	int	i;

	i = 0;
	if (vector)
	{
		while (vector[i])
		{
			free(vector[i]);
			i++;
		}
		free(vector);
	}
}

/**
 * @brief Frees memory allocated for a list node and its associated token.
 * 
 * Safely deallocates a given list node along with its associated `t_token`
 * structure and the token's `value` string. This function prevents memory
 * leaks by freeing each component individually, starting with the token value,
 * then the token itself, and finally the node.
 * 
 * @param node Pointer to the list node to be freed.
 */
void	ft_free_content(t_list *node)
{
	t_token	*token;

	if (node)
	{
		token = node->content;
		if (token)
		{
			if (token->value)
				free(token->value);
			free(token);
		}
		free(node);
	}
}

/**
 * @brief Frees all nodes in a linked list and their associated content.
 *
 * This function iterates through a linked list, releasing the memory 
 * allocated for each node and its associated content. The list itself 
 * is also freed at the end.
 *
 * @param list A pointer to the head of the linked list to be freed. 
 *        If `NULL`, the function does nothing.
 */
void	ft_free_list(t_list *list)
{
	t_list	*current;

	if (!list)
		return ;
	while (list)
	{
		current = list->next;
		ft_free_content(list);
		list = current;
	}
	free(list);
}

/**
 * @brief Cleans up resources used by a child process in the shell.
 *
 * This function is responsible for releasing memory and closing file
 * descriptors associated with a child process. It performs the following
 * actions:
 * - Frees the `args` vector if it is not NULL.
 * - If `cmd` equals 1, closes original file descriptors in the shell.
 * - Frees the shell structure `sh` if it is not NULL.
 *
 * @param sh The shell structure to clean up.
 * @param args The argument vector to be freed.
 * @param cmd Indicates whether to close original file descriptors (1 to close).
 */
void	ft_child_cleaner(t_shell *sh, char **args, int cmd)
{
	if (args)
		ft_free_vector(args);
	if (cmd == 1)
		close_original_fds(sh);
	if (sh)
		ft_free_sh(sh);
}

/**
 * @brief Frees the memory allocated for a shell structure.
 *
 * This function releases the memory allocated for the members of a t_shell
 * structure, including global and local environment variables and the heredoc
 * list. Finally, it frees the memory allocated for the t_shell structure
 * itself.
 *
 * @param sh Pointer to the t_shell structure to be freed.
 */
void	ft_free_sh(t_shell *sh)
{
	if (sh->global)
		ft_free_vector(sh->global);
	if (sh->local)
		ft_free_vector(sh->local);
	if (sh->limbo)
		ft_free_vector(sh->limbo);
	if (sh->heredoc_list)
	{
		ft_lstclear(sh->heredoc_list, free);
		free(sh->heredoc_list);
	}
	if (sh->root)
	{
		ft_free_tree(sh->root);
		sh->root = NULL;
	}
	close_original_fds(sh);
	free(sh);
}
