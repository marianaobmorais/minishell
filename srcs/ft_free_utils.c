#include "../includes/minishell.h"

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
	}
	free(vector);
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

void	ft_child_cleaner(t_shell *sh, char **args, int cmd)
{
	if (args)
		ft_free_vector(args);
	if (cmd == 1)
	{
		if (sh->stdin_ != -1)
		{
			close(sh->stdin_);
			sh->stdin_ = -1;
		}
		if (sh->stdout_ != -1)
		{
			close(sh->stdout_);
			sh->stdout_ = -1;
		}
	}
	if (sh)
		ft_free_sh(sh);
}
