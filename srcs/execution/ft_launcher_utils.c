#include "../../includes/minishell.h"

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
		if (sh->stdin_ == -1 || sh->stdout_ == -1)
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
			ft_stderror(TRUE, "Error restoring original FDs");
		//ft_stderror(FALSE, "Fechando .. %d", sh->stdin_); //debug
		if (sh->stdin_ != -1)
			close(sh->stdin_);
		if (dup2(sh->stdout_, STDOUT_FILENO) == -1)
			ft_stderror(TRUE, "Error restoring original FDs");
		if (sh->stdout_ != -1)
			close(sh->stdout_);
		//ft_stderror(FALSE, "Fechando .. %d", sh->stdout_); //debug
		while (waitpid(-1, NULL, 0) > 0)
			;
	}
	sh->fds_saved = 0;
}

/**
 * @brief Executes a single command if it is a built-in command.
 *
 * Checks if the given node is a built-in command, saves the original file
 * descriptors, processes redirections and arguments, and executes the command.
 * Restores the original file descriptors and returns the execution status.
 *
 * @param node The command node to be executed.
 * @param sh The shell structure containing the execution state and environment
 *
 * @return TRUE if the command was executed, FALSE otherwise.
 */
int	ft_single_command(void *node, void *next_node, t_shell *sh)
{
	void	*curr;
	char	**new_args;
	int		argc;

	if (ft_isjustbuiltin(node, sh))
	{
		ft_save_original_fds(sh);
		curr = ((t_node *)node)->left;
		while (ft_redir(((t_redir *)curr), next_node, sh))
			curr = ((t_redir *)curr)->next;
		ft_process_token_list(((t_exec *)curr)->args, ft_merge_env(sh->global, sh->local));
		new_args = tokentostring(((t_exec *)curr)->args);
		if (((t_exec *)curr)->type == EXPORT
			|| ((t_exec *)curr)->type == EXPORT_AP)
		{
			argc = ft_argslen(new_args);
			ft_export(argc, new_args, sh, LOCAL);
		}
		if (((t_exec *)curr)->type == EXEC)
			if (ft_isbuiltin(new_args))
				ft_exec_builtin(new_args, sh);
		ft_free_vector(new_args);
		ft_restore_original_fds(sh);
		return (TRUE);
	}
	return (FALSE);
}
