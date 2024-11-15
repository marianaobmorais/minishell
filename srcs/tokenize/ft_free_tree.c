#include "../../includes/minishell.h"

static void	ft_free_pipe(t_pipe *pipe)
{
	if (!pipe)
		return ;
	ft_free_tree(pipe->left);
	ft_free_tree(pipe->right);
	free(pipe);
}

static void	ft_free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->args)
		ft_free_vector(exec->args);
	free(exec);
}

static void	ft_free_redir(t_redir *redir)
{
	if (!redir)
		return ;
	ft_free_redir(redir->next);
	free(redir);
}

void	ft_free_tree(void *root)
{
	int	type;

	if (!root)
		return ;
	type = *(int *)root;
	if (type == PIPE)
		ft_free_pipe((t_pipe *)root);
	else if (type == EXEC || type == EXPORT || type == EXPORT_AP)
		ft_free_exec((t_exec *)root);
	else if (type == OUTFILE || type == INFILE || type == APPEND || type == HEREDOC)
		ft_free_redir((t_redir *)root);
}