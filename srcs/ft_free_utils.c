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
 * @brief Frees all elements of a linked list and the list itself.
 *
 * This function iterates through the linked list, freeing each node's content
 * and the node itself using the `ft_lstclear` function. After clearing the list,
 * it frees the list pointer and sets it to NULL to prevent dangling pointers.
 *
 * @param list A pointer to the linked list to be freed.
 */
void	ft_free_list(t_list **list)
{
	t_list	*current;
	t_list	*node;
	t_token	*token;

	if (!list)
		return ;
	while (*list)
	{
		current = (*list)->next;
		node = *list;
		token = node->content;
		if (node)
		{
			if (token->value)
			{
				free(token->value);
				token->value = NULL; //
				free(token);
				token = NULL; //
			}
			free(node);
			node = NULL;
		}
		*list = current;
	}	
	free(list);
	list = NULL;
}

