/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:41:55 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/26 17:38:20 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*sh;

	(void) argv;
	if (argc != 1)
		return (ft_stderror(FALSE, ERROR_INIT), 1);
	sh = ft_init_sh(envp);
	if (!sh)
		return (1);
	ft_handle_lvl(sh);
	ft_cli(sh);
	ft_free_sh(sh);
	exit(ft_exit_status(0, FALSE, FALSE));
}
