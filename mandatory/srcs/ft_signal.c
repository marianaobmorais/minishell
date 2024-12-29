/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:32:33 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/29 14:16:01 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Signal handler for the parent process, handling interrupts (SIGINT).
 *
 * This function handles the SIGINT signal in the parent process, typically
 * triggered by pressing `Ctrl + C`. It performs the following actions:
 * - Writes a newline character to standard output.
 * - Sets the exit status to 130, indicating that the process was terminated 
 *   by SIGINT.
 * - Resets the current input line and refreshes the `readline` prompt.
 *
 * @param sig The signal number received, expected to be SIGINT (signal 2).
 */
static void	sig_parent_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	ft_exit_status(130, TRUE, FALSE);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Signal handler for child processes, managing SIGINT and SIGQUIT
 *       signals.
 *
 * This function handles specific signals for child processes:
 * - If `SIGINT` (signal 2) is received, it writes a newline character to 
 *   standard output.
 * - If `SIGQUIT` (signal 3) is received, it outputs "Quit (core dumped)"
 *   to standard output.
 * 
 * This provides feedback when a child process is interrupted (`Ctrl + C`) or 
 * quit (`Ctrl + \`), ensuring proper message output without affecting the
 * parent process.
 *
 * @param sig The signal number received, which can be SIGINT or SIGQUIT.
 */
static void	sig_child_handler(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	if (sig == SIGQUIT)
		ft_putstr_fd_len("Quit (core dumped)\n", 2, 20);
	if (sig == SIGPIPE)
		ft_exit_status(141, TRUE, FALSE);
}

/**
 * @brief Resets critical signals to default behavior or ignores them.
 *
 * Configures key signals: ignores SIGTSTP and restores default behavior 
 * for SIGPIPE, SIGQUIT, and SIGINT.
 */
static void	sig_default(void)
{
	signal(SIGTSTP, SIG_IGN);
	signal(SIGPIPE, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

/**
 * @brief Configures signal handling based on process type (PARENT, HEREDOC,
 *        DEFAULT, CHILD).
 *
 * This function sets up signal handlers depending on the type of process:
 * - For `PARENT`, it ignores `SIGTSTP` and `SIGQUIT`, and handles `SIGINT` 
 *   with `sig_parent_handler`.
 * - For `HEREDOC`, it ignores `SIGQUIT` and leaves space to set up additional
 *   handlers as needed.
 * - For `DEFAULT`, it restores default behavior for `SIGQUIT` and `SIGINT`.
 * - For `CHILD`, it handles `SIGINT` and `SIGQUIT` with `sig_child_handler`.
 * 
 * Using different handlers allows precise control over parent, child, and 
 * heredoc process behaviors, ensuring proper signal response for interactive
 * shells and subprocesses.
 *
 * @param type Specifies the type of process (PARENT, HEREDOC, DEFAULT, CHILD).
 */
void	ft_signal(int type)
{
	if (type == PARENT_)
	{
		signal(SIGTSTP, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_parent_handler);
	}
	if (type == CHILD_)
	{
		signal(SIGTSTP, SIG_IGN);
		signal(SIGINT, sig_child_handler);
		signal(SIGQUIT, sig_child_handler);
	}
	if (type == HEREDOC_CHILD)
		sig_heredoc_child();
	if (type == HEREDOC_PARENT)
		sig_heredoc_parent();
	if (type == DEFAULT_)
	{
		sig_default();
		signal(SIGPIPE, sig_child_handler);
	}
}
