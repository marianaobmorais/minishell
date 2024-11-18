#include "../../includes/minishell.h"

int	ft_open(int type, char *pathname, int mode)
{
	int	fd;

	if (type == INFILE)
	{
		if (access(pathname, F_OK) == -1 || access(pathname, R_OK) == -1)
		{
			ft_error_handler();
			exit (1); // tratar
		}
	}
	fd = open(pathname, mode, 0644);
	if (fd == -1)
	{
		ft_error_handler();
		exit (1); // tratar
	}
	return (fd);
}

int	ft_redir(t_redir *node, char **my_envp)
{
	int	fd;

	if (node->type == OUTFILE || node->type == INFILE || node->type == APPEND)
	{
		fd = ft_open(node->type, node->target->value, node->mode);
		dup2(fd, STDIN_FILENO);
		close(fd);
		return (1);
	}
	else if (node->type == HEREDOC)
	{
		heredoc_fd(node->target->value, my_envp, node->target->state);
		return (1);
	}
	return (0);
}
