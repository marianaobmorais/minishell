#include "../../includes/minishell.h"

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
	if (dup2(sh->stdin_, STDIN_FILENO) == -1
		|| dup2(sh->stdout_, STDOUT_FILENO) == -1)
	{
		ft_stderror(TRUE, "Error restoring original FDs");
	}
	close(sh->stdin_);
	close(sh->stdout_);
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
int	ft_single_command(void *node, t_shell *sh)
{
	void	*curr_node;
	char	**new_args;

	if (ft_isjustbuiltin(node, sh))
	{
		ft_save_original_fds(sh);
		curr_node = ((t_node *)node)->left;
		while (ft_redir(((t_redir *)curr_node), sh))
			curr_node = ((t_redir *)curr_node)->next;
		//ft_process_token_list(((t_exec *)curr_node)->args, ft_merge_env(sh));
		new_args = tokentostring(((t_exec *)curr_node)->args);
		if (((t_exec *)curr_node)->type == EXPORT
			|| ((t_exec *)curr_node)->type == EXPORT_AP)
			ft_export(ft_argslen(new_args), new_args, sh, LOCAL);
		if (((t_exec *)curr_node)->type == EXEC)
		{
			if (ft_isbuiltin(new_args))
				ft_exec_builtin(new_args, sh);
		}
		ft_free_vector(new_args);
		ft_restore_original_fds(sh);
		return (TRUE);
	}
	return (FALSE);
}
