#include "../includes/minishell.h"

/**
 * @brief Frees all strings in a vector (array of strings) and the vector itself.
 *
 * This function iterates through the array of strings (vector), freeing each string
 * individually. After all strings are freed, the function frees the vector pointer
 * itself to prevent memory leaks.
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
 * Safely deallocates a given list node along with its associated `t_token` structure and
 * the token's `value` string. This function prevents memory leaks by freeing each component
 * individually, starting with the token value, then the token itself, and finally the node.
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
 * @brief Frees all elements of a linked list and the list itself.
 *
 * Iterates through each node in the provided list, freeing each node and its associated
 * `t_token` structure using `ft_free_node`. Once all nodes are freed, the list pointer
 * itself is deallocated, set to NULL to prevent dangling pointers, ensuring complete
 * cleanup of the list and its elements.
 *
 * @param list Double pointer to the head of the list to be freed.
 */
void	ft_free_list(t_list **list)
{
	t_list	*current;
	t_list	*node;

	if (!list)
		return ;
	while (*list)
	{
		current = (*list)->next;
		node = *list;
		ft_free_content(node);
		node = NULL;
		*list = current;
	}	
	free(list);
	list = NULL;
}

