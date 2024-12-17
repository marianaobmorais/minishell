/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:42:01 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:42:02 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*sh;

	(void) argv;
	if (argc != 1)
		return (ft_stderror(FALSE, ERROR_INIT), 1);
	sh = ft_init_sh(envp);
	if (!sh)
		return (1);
	ft_cli(sh);
	ft_free_sh(sh);
	exit(ft_exit_status(0, FALSE, FALSE));
}
