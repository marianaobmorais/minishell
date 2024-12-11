#include "../includes/minishell.h"

/**
 * @brief Manages and retrieves the current exit status with optional update
 *        and program termination.
 *
 * This function maintains a static variable `current_status` to track the
 * exit status. If `write_` is `TRUE`, the function updates `current_status`
 * with the provided `exit_status`. If `exit_` is `TRUE`, the program
 * terminates using the value of `current_status` as the exit code. If
 * neither flag is set, the function simply returns the current status.
 *
 * @param exit_status The new exit status to set if `write_` is `TRUE`.
 * @param write_ A flag to determine if `exit_status` should update
 *        `current_status`.
 * @param exit_ A flag to indicate if the program should terminate with
 *        `current_status` as the exit code.
 * @return The current value of `current_status`.
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
