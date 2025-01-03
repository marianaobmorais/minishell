/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launcher_utils_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:25:52 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/20 14:19:42 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Closes the original file descriptors in the shell structure.
 *
 * This function checks and closes the stdin, stdout, and stderr file
 * descriptors if they are not already closed, and sets their values to -1
 * to indicate they are closed.
 *
 * @param sh A pointer to the shell structure containing the file descriptors.
 */
void	close_original_fds(t_shell *sh)
{
	if (sh->stdin_ != -1)
	{
		close(sh->stdin_);
		sh->stdin_ = -1;
	}
	if (sh->stdout_ != -1)
	{
		close(sh->stdout_);
		sh->stdout_ = -1;
	}
	if (sh->stderr_ != -1)
	{
		close(sh->stderr_);
		sh->stderr_ = -1;
	}
}

/**
 * @brief Closes the file descriptors in the provided array.
 *
 * This function checks and closes the file descriptors in the provided array
 * if they are not already closed, and sets their values to -1 to indicate they
 * are closed.
 *
 * @param fds An array of file descriptors to be closed. The array should have
 *            at least two elements.
 */
void	close_fds(int *fds)
{
	if (fds[0] != -1)
	{
		close(fds[0]);
		fds[0] = -1;
	}
	if (fds[1] != -1)
	{
		close(fds[1]);
		fds[1] = -1;
	}
}

/**
 * @brief Saves the original file descriptors for stdin and stdout.
 *
 * Duplicates the original stdin and stdout file descriptors and stores them
 * in the shell structure if they haven't been saved already. Handles errors
 * and updates the shell state.
 *
 * @param sh The shell structure containing the file descriptors and state.
 */
void	ft_save_original_fds(t_shell *sh)
{
	if (sh->fds_saved == 0)
	{
		sh->stdin_ = dup(STDIN_FILENO);
		sh->stdout_ = dup(STDOUT_FILENO);
		sh->stderr_ = dup(STDERR_FILENO);
		if (sh->stdin_ == -1 || sh->stdout_ == -1 || sh->stderr_ == -1)
		{
			ft_stderror(TRUE, "Error saving original FDs");
			ft_exit_status(1, TRUE, TRUE);
		}
		sh->fds_saved = 1;
	}
}

/**
 * @brief Restores the original file descriptors for stdin and stdout.
 *
 * Duplicates the original stdin and stdout file descriptors back to their
 * standard locations. Handles errors and closes the saved file descriptors.
 *
 * @param sh The shell structure containing the saved file descriptors.
 */
void	ft_restore_original_fds(t_shell *sh)
{
	if (sh->fds_saved == 1)
	{
		if (dup2(sh->stdin_, STDIN_FILENO) == -1)
			ft_stderror(TRUE, "Error restoring original STDIN");
		if (sh->stdin_ != -1)
			close(sh->stdin_);
		if (dup2(sh->stdout_, STDOUT_FILENO) == -1)
			ft_stderror(TRUE, "Error restoring original STDOUT");
		if (sh->stdout_ != -1)
			close(sh->stdout_);
		if (dup2(sh->stderr_, STDERR_FILENO) == -1)
			ft_stderror(TRUE, "Error restoring original STDERR");
		if (sh->stderr_ != -1)
			close(sh->stderr_);
		while (waitpid(-1, NULL, 0) > 0)
			;
	}
	sh->fds_saved = 0;
}

/*
 * @brief Handles sub-root node relationships in the syntax tree.
 *
 * This function processes the left child of a given node to determine 
 * if it is of type `SUB_ROOT` or part of a redirection chain. 
 * If the left child is of type `SUB_ROOT`, the shell's `next_node` pointer 
 * is updated to point to the right child. Additionally, the function ensures 
 * proper handling of redirection nodes that may lead to a `SUB_ROOT` type.
 * 
 * The function also resets the `next_node` pointer to `NULL` if the current 
 * node matches the `next_node`, preventing unwanted behavior in subsequent 
 * syntax tree processing.
 *
 * @param node The current node being processed in the syntax tree.
 * @param sh The shell structure containing the `next_node` and `prev_nnode`
 *        pointers.
 */
void	ft_issubroot(t_node *node, t_shell *sh)
{
	t_redir	*redir;

	if (((t_node *) node->left)->type == SUB_ROOT)
		sh->next_node = node->right;
	else if (ft_is_node_type(node->left, REDIR) == true)
	{
		redir = (t_redir *) node->left;
		while (redir)
		{
			if (ft_is_node_type((t_node *)redir, REDIR) == true
				&& redir->next && ((t_node *)(redir->next))->type == SUB_ROOT)
			{
				sh->prev_nnode = (t_node *) redir;
				sh->next_node = node->right;
			}
			if (ft_is_node_type((t_node *)redir, REDIR) == false)
				break ;
			redir = redir->next;
		}
	}
	if (node == sh->next_node)
		sh->next_node = NULL;
}
