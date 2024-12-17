/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:32:29 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:32:30 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Constructs and sets the shell prompt based on the current directory.
 *
 * This function retrieves the current working directory and constructs the
 * shell prompt by combining predefined prompt parts with the current path.
 * The constructed prompt is then assigned to the `prompt` field of the
 * `t_shell` structure.
 *
 * @param sh A pointer to the `t_shell` structure containing the shell state.
 */
void	ft_prompt(t_shell *sh)
{
	char	*prompt;
	char	current_path[OPENFD_MAX];

	getcwd(current_path, OPENFD_MAX);
	prompt = ft_strjoin(PROMPT_1, current_path);
	prompt = merge(prompt, PROMPT_2);
	sh->prompt = prompt;
}
