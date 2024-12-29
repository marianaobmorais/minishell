/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:32:02 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/29 14:17:45 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Reads input for a here-document until the end-of-file delimiter.
 *
 * Continuously reads user input, processes it, and writes it to a file
 * descriptor until the specified end-of-file string is encountered. Handles
 * signal interruptions and expands env variables if in the general state.
 *
 * @param eof The end-of-file delimiter string.
 * @param state The state indicating if env variable expansion is needed.
 * @param my_envp Array of environment variables for expansion.
 * @param fd_write The file descriptor to write the input to.
 */
static void	read_heredoc(char *eof, int state, char **my_envp, int fd_write)
{
	char	*input;

	input = NULL;
	ft_signal(HEREDOC_CHILD);
	while (ft_exit_status(0, FALSE, FALSE) == 0)
	{
		input = readline("> ");
		if (ft_exit_status(0, FALSE, FALSE) == 130)
			return (free(input));
		if (!input || !ft_strcmp(eof, input))
		{
			if (!input)
				ft_stderror(FALSE, "warning: here-document at line %d"\
				" delimited by end-of-file (wanted `%s')", count_line(0), eof);
			free(input);
			break ;
		}
		add_history(input);
		if (state == GENERAL)
			input = ft_expand_input(input, my_envp);
		ft_putendl_fd(input, fd_write);
		count_line(1);
		if (input)
			free(input);
	}
}

/**
 * @brief Saves the contents of a file descriptor to a file.
 *
 * Opens the specified pathname for writing, reads from the provided file
 * descriptor, and writes the data to the file. Handles errors, manages memory,
 * and updates the shell's here-document list.
 *
 * @param pathname The path to the file to save the contents.
 * @param fd The file descriptor to read from.
 * @param sh The shell structure containing the here-document list.
 */
static void	save_heredoc(char *pathname, int fd, t_shell *sh)
{
	int		fd_save;
	int		rd;
	char	*buffer;

	fd_save = open(pathname, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_save == -1)
		return (ft_exit_status(1, TRUE, FALSE), ft_stderror(TRUE, ""));
	buffer = malloc(BUFFER_SIZE * sizeof(char *) + 1);
	if (!buffer)
		return (ft_exit_status(1, TRUE, FALSE), ft_stderror(TRUE, ""));
	rd = 1;
	while (rd != 0)
	{
		rd = read(fd, buffer, BUFFER_SIZE);
		buffer[rd] = '\0';
		if (rd <= 0)
			break ;
		write(fd_save, buffer, ft_strlen(buffer));
	}
	free(buffer);
	close(fd);
	close(fd_save);
	ft_lstadd_back(sh->heredoc_list, ft_lstnew((ft_strdup(pathname))));
	free(pathname);
}

/**
 * @brief Waits for a here-document process to finish and updates exit status.
 *
 * Waits for the specified process ID to finish. If the process exits normally,
 * sets the exit status. If the process is terminated by a signal, sets the
 * exit status to the signal number plus 128.
 *
 * @param pid The process ID of the here-document process to wait for.
 */
static void	wait_heredoc(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) != -1)
	{
		if (WIFEXITED(status))
			ft_exit_status(WEXITSTATUS(status), TRUE, FALSE);
		else if (WIFSIGNALED(status))
			ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE);
	}
}

/**
 * @brief Creates a pipe for here-document input and manages the process.
 *
 * Sets up a pipe, forks a process to read the here-document input, and waits
 * for the child process to finish. If the child process succeeds, saves the
 * here-document content to a file and updates the shell state.
 *
 * @param eof The end-of-file delimiter string.
 * @param my_envp Array of environment variables for expansion.
 * @param state The state indicating if env variable expansion is needed.
 * @param sh The shell structure containing the here-document list.
 *
 * @return TRUE if the here-doc was successfully processed, FALSE otherwise.
 */
static int	heredoc_fd(char *eof, char **my_envp, t_state state, t_shell *sh)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), FALSE);
	pid = fork();
	if (pid == -1)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), FALSE);
	if (pid == 0)
	{
		close(fd[0]);
		ft_exit_status(0, TRUE, FALSE);
		read_heredoc(eof, state, my_envp, fd[1]);
		close(fd[1]);
		ft_child_cleaner(sh, my_envp, 0);
		if (ft_exit_status(0, FALSE, FALSE) != 0)
			ft_exit_status(130, TRUE, TRUE);
		ft_exit_status(0, TRUE, TRUE);
	}
	close(fd[1]);
	wait_heredoc(pid);
	if (ft_exit_status(0, FALSE, FALSE) != 0)
		return (close(fd[0]), ft_free_vector(my_envp), FALSE);
	save_heredoc(ft_create_pathname(), fd[0], sh);
	return (close(fd[0]), ft_free_vector(my_envp), TRUE);
}

/**
 * @brief Searches and processes here-documents in a command tree.
 *
 * Traverses a command tree or list of redirections, processing
 * here-documents when found. Handles different node types and
 * updates the shell state accordingly.
 *
 * @param node The current node in the command tree or redirection list.
 * @param sh The shell structure containing the execution state and environment
 */
void	ft_search_heredoc(void *node, t_shell *sh)
{
	t_redir	*rnd;
	t_token	*tnd;
	t_state	st;

	if (!node)
		return ;
	else if (((t_node *)node)->type == PIPE)
	{
		ft_search_heredoc(((t_node *)node)->left, sh);
		return (ft_search_heredoc(((t_node *)node)->right, sh));
	}
	else if (((t_redir *)node)->type == HEREDOC && sh->run == TRUE)
	{
		rnd = (t_redir *)node;
		ft_process_token_list(rnd->target, ft_merge_env(sh->global, sh->local));
		tnd = (t_token *)(*rnd->target)->content;
		st = tnd->state;
		sh->run = heredoc_fd(tnd->value, \
			ft_merge_env(sh->global, sh->local), st, sh);
	}
	else if (((t_redir *)node)->type == EXEC
		|| ((t_redir *)node)->type == EXPORT
		|| ((t_redir *)node)->type == EXPORT_AP)
		return ;
	ft_search_heredoc(((t_redir *)node)->next, sh);
}
