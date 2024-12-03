#include "../../includes/minishell.h"

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

void	ft_restore_original_fds(t_shell *sh)
{
	if (dup2(sh->stdin_, STDIN_FILENO) == -1 || dup2(sh->stdout_, STDOUT_FILENO) == -1)
	{
		ft_stderror(TRUE, "Error restoring original FDs");
	}
	close(sh->stdin_);
	close(sh->stdout_);
}

int	ft_single_command(void *node, t_shell *sh)
{
	void	*curr_node;
	char	**new_args;

	if (ft_isjustbuiltin(node, sh))
	{
		if (!sh->fds_saved) {
			ft_save_original_fds(sh);
			sh->fds_saved = 1;
		}
		curr_node = ((t_node *)node)->left;
		while (ft_redir(((t_redir *)curr_node), sh))
			curr_node = ((t_redir *)curr_node)->next;
		//ft_process_token_list(((t_exec *)curr_node)->args, ft_merge_env(sh)); //ft_merge_env
		new_args = tokentostring(((t_exec *)curr_node)->args);
		if (((t_exec *)curr_node)->type == EXPORT || ((t_exec *)curr_node)->type == EXPORT_AP)
		{
			int len = ft_argslen(new_args);
			ft_export(len, new_args, sh, LOCAL);
		}
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
