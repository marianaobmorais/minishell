#include "../../includes/minishell.h"

int	ft_open(int type, char *pathname, int mode)
{
	int	fd;

	if (type == INFILE)
	{
		if (access(pathname, F_OK) == -1 || access(pathname, R_OK) == -1)
		{
			ft_stderror(TRUE, "%s: ", pathname);
			ft_exit_status(1, TRUE, FALSE);
			return (-1);
		}
	}
	fd = open(pathname, mode, 0644);
	if (fd == -1)
	{
		ft_stderror(TRUE, "%s: ", pathname);
		ft_exit_status(1, TRUE, FALSE);
	}
	return (fd);
}

int	ft_redir(t_redir *node, char **my_envp, int *fds, t_redir *next_node, t_shell *sh) //alterar nome args
{
	int	fd;
	(void)fds;
	(void)next_node;

	if (node->type == OUTFILE || node->type == INFILE || node->type == APPEND)
	{
		ft_process_token_list(node->target, my_envp);
		fd = ft_open(node->type, ((t_token *)(*node->target)->content)->value, node->mode);
		if (fd == -1)
			return (FALSE);
		if (node->type == OUTFILE || node->type == APPEND)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
		close(fd);
		return (TRUE);
	}
	else if (node->type == HEREDOC)
	{
		if (sh->heredoc)
			dup2(sh->stdin_, STDIN_FILENO);
		ft_process_token_list(node->target, my_envp);
		fd = heredoc_fd(((t_token *)(*node->target)->content)->value, my_envp, ((t_token *)node->target)->state);
		//if (next_node->type != HEREDOC)
		dup2(fd, STDIN_FILENO);
		close(fd);
		sh->heredoc = TRUE;
		return (TRUE);
	}
	return (FALSE);
}
