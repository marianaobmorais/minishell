#include "../../includes/minishell.h"

int	ft_open(int type, char *pathname, int mode)
{
	int	fd;

	if (type == INFILE)
	{
		if (access(pathname, F_OK) == -1 || access(pathname, R_OK) == -1)
		{
			ft_stderror(TRUE, "%s: ", pathname);
			ft_exit_status(1, TRUE, TRUE);
		}
	}
	fd = open(pathname, mode, 0644);
	if (fd == -1)
	{
		ft_stderror(TRUE, "%s: ", pathname);
		ft_exit_status(1, TRUE, TRUE);
	}
	return (fd);
}

int	ft_redir(t_redir *node, char **my_envp)
{
	int	fd;

	ft_process_token_list(node->target, my_envp);
	if (node->type == OUTFILE || node->type == INFILE || node->type == APPEND)
	{
		fd = ft_open(node->type, ((t_token *)node->target)->value, node->mode);
		if (node->type == OUTFILE || node->type == APPEND)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
		close(fd);
		return (TRUE);
	}
	else if (node->type == HEREDOC)
	{
		heredoc_fd(((t_token *)node->target)->value, my_envp, ((t_token *)node->target)->state);
		return (TRUE);
	}
	return (FALSE);
}
