#include "../includes/minishell.h"

/**
 * @brief Manages and retrieves the current exit status, with optional update and exit behavior.
 *
 * This function stores and retrieves a static exit status variable. If `write_` 
 * is set to `TRUE`, the function updates `current_status` to the provided `exit_status`. 
 * If `exit_` is `TRUE`, the function will terminate the program using the stored `current_status` 
 * as the exit code. Otherwise, it simply returns the current status.
 *
 * @param exit_status The new exit status to set if `write_` is `TRUE`.
 * @param write_ A flag indicating if `exit_status` should be written to `current_status`.
 * @param exit_ A flag indicating if the program should exit with `current_status`.
 * @return The current exit status stored in `current_status`.
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
