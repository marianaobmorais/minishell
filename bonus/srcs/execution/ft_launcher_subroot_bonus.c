/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launcher_subroot_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:58:37 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/28 23:43:59 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Executes a subroot node in the syntax tree.
 *
 * This function manages the execution of a specific subroot node within the
 * syntax tree. It coordinates the launcher, cleans up resources in the child
 * process, and updates the shell's exit status accordingly.
 *
 * @param node The syntax tree node to be executed.
 * @param sh The shell structure containing the execution context and state.
 */
void	ft_exec_subroot(void *node, t_shell *sh)
{
	ft_launcher_manager(node, sh);
	ft_child_cleaner(sh, NULL, 0);
	ft_exit_status(0, FALSE, TRUE);
}

/**
 * @brief Handles the creation and execution of a child process for a subroot.
 *
 * This function forks a new process to execute a subroot node of the syntax
 * tree. In the child process, it manages file descriptor redirection,
 * evaluates the next node for proper output handling, and invokes the
 * execution of the subroot. The parent process simply returns the PID of
 * the created child process.
 *
 * @param fds An array of file descriptors for communication and redirection.
 * @param sh The shell structure containing the execution context and state.
 * @param node The syntax tree node to execute in the child process.
 * @param next_node The next node in the syntax tree to evaluate for output
 *                  redirection.
 * @return The process ID of the created child process.
 */
pid_t	ft_child_process_subroot(int *fds, t_shell *sh, void *node, \
	void *next_node)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_stderror(TRUE, "");
		ft_exit_status(1, TRUE, FALSE);
	}
	if (pid == 0)
	{
		close_original_fds(sh);
		close(fds[0]);
		if (next_node != NULL && (sh->prev && ((t_redir *)sh->prev)->type
				!= OUTFILE) && ((t_redir *)sh->prev)->type != APPEND)
		{
			dup2(fds[1], STDOUT_FILENO);
		}
		close(fds[1]);
		sh->fds_saved = 0;
		ft_exec_subroot(node, sh);
	}
	return (pid);
}

/**
 * @brief Manages the parent process after forking a subroot execution.
 *
 * This function handles the necessary operations in the parent process after
 * a subroot node has been executed in a child process. It manages signal
 * handling, waits for the child process to complete, updates the exit status,
 * and restores the original file descriptors. For additional nodes, it sets
 * up redirection and launches the next part of the syntax tree.
 *
 * @param fds An array of file descriptors used for communication and
 *            redirection.
 * @param sh The shell structure containing the execution context and state.
 * @param node The syntax tree node to process after the subroot execution.
 * @param pid The process ID of the child process to wait for.
 */
void	ft_parent_process_subroot(int *fds, t_shell *sh, void *node, pid_t pid)
{
	int		status;

	sh->error_fd = 0;
	if (!node)
	{
		ft_signal(CHILD_);
		close_fds(fds);
		if (pid != -1 && waitpid(pid, &status, 0) != -1)
		{
			if (WIFEXITED(status))
				ft_exit_status(WEXITSTATUS(status), TRUE, FALSE);
			else if (WIFSIGNALED(status))
				ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE);
		}
		return (ft_restore_original_fds(sh));
	}
	if (node)
		dup2(fds[0], STDIN_FILENO);
	close_fds(fds);
	if ((sh->prev && ft_is_node_type(sh->prev, REDIR_OUT)) || (sh->next_node
			&& sh->prev_nnode && ft_is_node_type(sh->prev_nnode, REDIR_OUT)))
		dup2(sh->stdout_, STDOUT_FILENO);
	ft_launcher(node, ((t_node *)node)->right, NULL, sh);
}

/**
 * @brief Launches the execution of a subroot node within the syntax tree.
 *
 * This function coordinates the creation of a new process to execute a
 * subroot node. It initializes a pipe for inter-process communication,
 * delegates the child process execution to `ft_child_process_subroot`, and
 * manages the parent process responsibilities via `ft_parent_process_subroot`.
 * If an error occurs during pipe creation, it updates the exit status and
 * outputs an error message.
 *
 * @param node The syntax tree node to be executed.
 * @param next_node The next node in the syntax tree, used for managing
 *                  redirection and execution flow.
 * @param sh The shell structure containing the execution context and state.
 */
void	ft_launcher_subroot(void *node, void *next_node, t_shell *sh)
{
	pid_t	pid;
	int		new_fds[2];

	pid = -1;
	if (sh->error_fd == 0)
	{
		if (pipe(new_fds) == -1)
			return (ft_exit_status(1, TRUE, FALSE), ft_stderror(TRUE, ""));
		pid = ft_child_process_subroot(new_fds, sh, node, next_node);
	}
	ft_parent_process_subroot(new_fds, sh, next_node, pid);
}
