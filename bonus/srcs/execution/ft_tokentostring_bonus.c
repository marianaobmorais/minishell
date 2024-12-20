/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokentostring_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 16:00:04 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/20 16:04:18 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Safely allocates memory for an array of strings.
 *
 * This function allocates memory for an array of strings, ensuring that the
 * allocation is successful. If the allocation fails, the `ft_error_malloc`
 * function is called to handle the error.
 *
 * @return A pointer to the allocated array of strings.
 */
static char	**safe_malloc_str(void)
{
	char	**n_args;

	n_args = (char **)malloc(sizeof(char *));
	if (!n_args)
		ft_error_malloc("new_args");
	*n_args = NULL;
	return (n_args);
}

/**
 * @brief Converts a list of tokens to an array of strings.
 *
 * Iterates through a linked list of tokens, converting each token's value
 * to a string and storing it in a newly allocated array. Handles memory
 * allocation and error checking.
 *
 * @param args A double pointer to the list of tokens.
 *
 * @return A newly allocated array of strings, or NULL on error.
 */
char	**tokentostring(t_list **args)
{
	char	**n_args;
	char	**new_args_cp;
	t_list	*curr;
	int		i;

	n_args = safe_malloc_str();
	curr = *args;
	while (curr)
	{
		if (((t_token *)(curr)->content)->expand
			&& !((t_token *)(curr)->content)->state)
		{
			new_args_cp = ft_split(((t_token *)(curr)->content)->value, ' ');
			i = -1;
			while (new_args_cp[++i])
				n_args = ft_add_to_vector(n_args, new_args_cp[i]);
			ft_free_vector(new_args_cp);
		}
		else
			n_args = ft_add_to_vector(n_args,
					((t_token *)(curr)->content)->value);
		curr = (curr)->next;
	}
	return (n_args);
}
