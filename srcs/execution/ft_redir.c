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
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	ft_lstdelone((*sh->heredoc_list), free);
	*sh->heredoc_list = tmp;
	unlink(pathname);
}

int	ft_redir(t_redir *node, t_shell *sh)
{
	int		fd;
	t_token	*tnode;

	if (node->type == OUTFILE || node->type == INFILE || node->type == APPEND)
	{
		ft_process_token_list(node->target, sh->global); //ft_merge_env
		tnode = (t_token *)(*node->target)->content;
		fd = ft_open(node->type, tnode->value, node->mode);
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
		ft_redir_heredoc(sh, node);
		return (TRUE);
	}
	return (FALSE);
}
