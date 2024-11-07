#include "../includes/minishell.h"

/* 
* @brief Get exit status code
* 
* 
*/
int	ft_exit_status(int status, int write_, int exit_)
{
	static int	current_status;

	if (write_ == TRUE)
		current_status = status;
	if (exit_ == TRUE)
		exit(current_status);
	return (current_status);
}
