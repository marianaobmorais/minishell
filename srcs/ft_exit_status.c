#include "../includes/minishell.h"

/* 
* @brief Get exit status code
* 
* 
*/
int	ft_exit_status(int exit_status, int write_, int exit_)
{
	static int	current_status;

	if (write_ == TRUE)
		current_status = exit_status;
	if (exit_ == TRUE)
		exit(current_status);
	return (current_status);
}
