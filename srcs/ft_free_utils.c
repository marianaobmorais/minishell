#include "../includes/minishell.h"

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
	ft_lstclear(list, free);
	if (list)
		free(list);
	list = NULL;
}
