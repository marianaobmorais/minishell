#include "../../includes/minishell_bonus.h"

/**
 * @brief Opens a file with specified type and mode.
 *
 * Handles the opening of a file based on the given type and mode. For input
 * files,it checks for existence and read permissions before attempting to open
 * If the  * file cannot be accessed or opened, it logs an error and sets an
 * exit status.
 *
 * @param type The type of the file, such as INFILE.
 * @param pathname A string representing the path to the file.
 * @param mode The mode in which to open the file.
 *
 * @return The file descriptor on success, or -1 on error.
 */
int	ft_open(t_redir *node, char *pathname, int mode, t_shell *sh)
{
	int	fd;

	fd = -1;
	if (node->type == INFILE)
	{
		if (access(pathname, F_OK) == -1 || access(pathname, R_OK) == -1)
		{
			if (sh->error_fd == 0)
				ft_stderror(TRUE, "%s: ", pathname);
			sh->error_fd = -1;
			ft_exit_status(1, TRUE, FALSE);
			return (open("/dev/null", O_RDONLY));
		}
	}
	if (sh->error_fd == 0)
		fd = open(pathname, mode, 0644);
	if (fd == -1)
	{
		if (sh->error_fd == 0)
			ft_stderror(TRUE, "%s: ", pathname);
		sh->error_fd = -1;
		ft_exit_status(1, TRUE, FALSE);
		return (fd);
	}
	return (fd);
}

/**
 * @brief Searches for an execution node in branch.
 *
 * Iterates through a linked list of redirection nodes to check if any node
 * is of type EXEC. If an EXEC node is found, the function returns TRUE,
 * otherwise it returns FALSE.
 *
 * @param node A pointer to the first node in the redirection list.
 *
 * @return TRUE if an EXEC node is found, or FALSE otherwise.
 */
int	ft_search_exec(t_redir *node)
{
	int		exec;
	t_redir	*curr;

	exec = FALSE;
	curr = node->next;
	while (curr)
	{
		if (curr->type == EXEC)
		{
			exec = TRUE;
			break ;
		}
		curr = curr->next;
	}
	return (exec);
}

/**
 * @brief Handles here-document redirections.
 *
 * Manages here-document redirections by opening the temporary file,
 * duplicating its file descriptor to standard input, and then cleaning up
 * by removing the temporary file and updating the heredoc list.
 *
 * @param sh The shell structure containing the heredoc list.
 * @param node A pointer to the redirection node.
 */
void	ft_redir_heredoc(t_shell *sh, t_redir *node)
{
	char	*pathname;
	int		fd;
	t_list	*tmp;

	pathname = (char *)(*sh->heredoc_list)->content;
	tmp = (*sh->heredoc_list)->next;
	if (ft_search_exec(node) == TRUE)
	{
		fd = open(pathname, O_RDONLY);
		if (fd == -1)
		{
			fd = open("/dev/null", O_RDONLY);
			ft_stderror(TRUE, "%s: ", pathname);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		unlink(pathname);
	}
	free((*sh->heredoc_list)->content);
	free((*sh->heredoc_list));
	*sh->heredoc_list = tmp;
}

/**
 * @brief Handles file redirections for the shell.
 *
 * Processes different types of file redirections, including OUTFILE, INFILE,
 * APPEND, and HEREDOC. Opens the specified file and duplicates its file
 * descriptor to the appropriate standard stream. Handles here-document
 * redirections separately.
 *
 * @param nd A pointer to the redirection node.
 * @param sh The shell structure containing environment variables and settings.
 *
 * @return TRUE on success, or FALSE on failure.
 */
int	ft_redir(t_redir *nd, t_shell *sh)
{
	char	*target_tmp;
	int		fd;

	if (nd->type == OUTFILE || nd->type == INFILE || nd->type == APPEND)
	{
		target_tmp = ft_strdup(((t_token *)(*nd->target)->content)->value);
		ft_process_token_list(nd->target, ft_merge_env(sh->global, sh->local));
		if (!*nd->target)
			return (ft_stderror(FALSE, "%s: ambiguous redirect", target_tmp), \
				ft_exit_status(1, TRUE, FALSE), free(target_tmp), FALSE);
		fd = ft_open(nd, ((t_token *)(*nd->target)->content)->value, \
			nd->mode, sh);
		if (fd != -1)
		{
			if (nd->type == OUTFILE || nd->type == APPEND)
				dup2(fd, STDOUT_FILENO);
			else
				dup2(fd, STDIN_FILENO);
			close(fd);
		}
		return (free(target_tmp), TRUE);
	}
	else if (nd->type == HEREDOC)
		return (ft_redir_heredoc(sh, nd), TRUE);
	return (FALSE);
}
