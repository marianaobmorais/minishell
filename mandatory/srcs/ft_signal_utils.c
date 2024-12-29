/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:32:33 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/29 22:08:40 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles the `SIGINT` signal for the parent process during heredoc.
 *
 * This signal handler is invoked when a `SIGINT` signal is received while
 * processing a heredoc in the parent process. It writes a newline to the
 * standard output and updates the shell's exit status to 130, indicating
 * that the heredoc was interrupted by the user.
 *
 * @param sig The signal number received (unused in this function).
 */
static void	sig_heredoc_parent_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	ft_exit_status(130, TRUE, FALSE);
}

/**
 * @brief Handles the `SIGINT` signal for the child process during heredoc.
 *
 * This signal handler is invoked when a `SIGINT` signal is received in the
 * child process during heredoc processing. It closes the standard input to
 * terminate heredoc input and updates the shell's exit status to 130,
 * indicating that the operation was interrupted by the user.
 *
 * @param sig The signal number received.
 */
static void	sig_heredoc_child_handler(int sig)
{
	if (sig == SIGINT)
	{
		close(STDIN_FILENO);
		ft_exit_status(130, TRUE, FALSE);
	}
}

/**
 * @brief Configures signal handlers for the parent process during heredoc.
 *
 * This function sets up signal handling for the parent process while
 * processing a heredoc. It ignores `SIGTSTP`, `SIGPIPE`, and `SIGQUIT` signals
 * and assigns a custom handler for `SIGINT` to handle user interruptions
 * gracefully.
 */
void	sig_heredoc_parent(void)
{
	signal(SIGINT, sig_heredoc_parent_handler);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Configures signal handlers for the child process during heredoc.
 *
 * This function sets up signal handling for the child process while processing
 * a heredoc. It ignores `SIGTSTP` and `SIGQUIT` signals and assigns a custom
 * handler for `SIGINT` to handle user interruptions by closing the input and
 * updating the exit status.
 */
void	sig_heredoc_child(void)
{
	signal(SIGINT, sig_heredoc_child_handler);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Prints messages for signals based on the last exit status.
 *
 * This function checks the last exit status:
 * - If the status is 130, it prints a newline (`\n`) to indicate an interrupt
 *   signal (`SIGINT`).
 * - If the status is 131, it prints "Quit (core dumped)" to indicate a quit
 *   signal (`SIGQUIT`).
 *
 * This ensures appropriate feedback for user-triggered signals in the shell.
 */
void	ft_print_signal(void)
{
	if (ft_exit_status(0, FALSE, FALSE) == 130)
		write(1, "\n", 1);
	if (ft_exit_status(0, FALSE, FALSE) == 131)
		write(1, "Quit (core dumped)\n", 20);
}
