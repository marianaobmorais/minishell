#include "../../includes/minishell.h"

int	ft_redir(t_redir *node, char **my_envp)
{
	(void)node;
	if (node->type == OUTFILE)
		printf("outfile\n");
	else if (node->type == INFILE)
		printf("infile\n");
	else if (node->type == HEREDOC)
	{
		heredoc_fd(node->target->value, my_envp, node->target->state);
		return (1);
	}
	return (0);
}
