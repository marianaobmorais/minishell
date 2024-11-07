#include "../../includes/minishell.h"

void	ft_exit(int exit_)
{
	ft_putendl_fd("exit", 2);
	ft_exit_status(0, TRUE, exit_);
}
