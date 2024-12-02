#include "../../includes/minishell.h"

void	ft_save_original_fds(t_shell *sh)
{
	if (sh->fds_saved == 0)
	{
		sh->stdin_ = dup(STDIN_FILENO);
		sh->stdout_ = dup(STDOUT_FILENO);
		if (sh->stdin_ == -1 || sh->stdout_ == -1)
		{
			perror("Error saving original FDs");
			exit(EXIT_FAILURE);
		}
		sh->fds_saved = 1;
	}
}

void	ft_restore_original_fds(t_shell *sh)
{
	if (dup2(sh->stdin_, STDIN_FILENO) == -1 || dup2(sh->stdout_, STDOUT_FILENO) == -1)
	{
		perror("Error restoring original FDs");
	}
	close(sh->stdin_);
	close(sh->stdout_);
}

int	ft_single_command(void *node, t_env *env, t_shell *sh)
{
	void	*curr_node;
	char	**new_args;

	if (ft_isjustbuiltin(node, env))
	{
		if (!sh->fds_saved) {
			ft_save_original_fds(sh);
			sh->fds_saved = 1;
		}
		curr_node = ((t_pipe *)node)->left;
		while (ft_redir(((t_redir *)curr_node), env->global, sh))
			curr_node = ((t_redir *)curr_node)->next;
		if (((t_exec *)curr_node)->type == EXEC)
		{
			new_args = tokentostring(((t_exec *)curr_node)->args);
			if (ft_isbuiltin(new_args))
				ft_exec_builtin(new_args, env);
			ft_free_vector(new_args);
		}
		ft_restore_original_fds(sh);
		return (TRUE);
	}
	return (FALSE);
}
