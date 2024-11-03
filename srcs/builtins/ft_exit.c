#include "../../includes/minishell.h"

void	ft_exit(int exit_)
{
	printf("exit\n");
	ft_exit_status(0, TRUE, exit_);
}
