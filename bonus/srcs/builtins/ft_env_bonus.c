/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:02:09 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/26 15:59:54 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Prints the environment variables.
 *
 * This function iterates through the `envp` array and prints each
 * environment variable (each string) to the standard output. If the `envp` or 
 * `*envp` array is `NULL`, it calls the error handler to handle the situation
 * appropriately.
 *
 * @param envp The array of environment variables to print.
 */
void	ft_env(int argc, char **args, char **envp)
{
	int	i;

	i = 0;
	if (!envp || !*envp)
	{
		ft_stderror(FALSE, "env: No such file or directory");
		ft_exit_status(127, TRUE, FALSE);
		return ;
	}
	if (argc > 1)
	{
		ft_stderror(FALSE, "env: `%s`: No such file or directory", args[1]);
		ft_exit_status(127, TRUE, FALSE);
		return ;
	}
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	ft_exit_status(0, TRUE, FALSE);
}

/* int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	ft_env(envp);
	return (0);
} */
