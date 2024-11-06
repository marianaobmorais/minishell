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

void	ft_free_list(t_list **list)
{
	ft_lstclear(list, free);
	if (list)
		free(list);
	list = NULL;
}
