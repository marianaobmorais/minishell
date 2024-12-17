/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:31:47 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:31:48 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Prints the current working directory.
 *
 * This function retrieves the current working directory using `getcwd` 
 * and prints it to standard output. If an error occurs in retrieving 
 * the directory, it sets the exit status to 1. Otherwise, it sets the 
 * exit status to 0 to indicate success.
 *
 * @return An integer representing the exit status. Returns 1 if an error 
 * occurs, otherwise 0.
 */
int	ft_pwd(void)
{
	char	current_path[OPENFD_MAX];

	if (getcwd(current_path, OPENFD_MAX) != NULL)
		printf("%s\n", current_path);
	else
		return (ft_exit_status(1, TRUE, FALSE));
	return (ft_exit_status(0, TRUE, FALSE));
}
