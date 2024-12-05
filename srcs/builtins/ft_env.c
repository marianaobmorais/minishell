#include "../../includes/minishell.h"

/**
 * @brief Prints the environment variables.
 *
 * This function iterates through the `my_envp` array and prints each
 * environment variable (each string) to the standard output. If the `my_envp`
 * array is `NULL`, it calls the error handler to handle the situation
 * appropriately.
 *
 * @param my_envp The array of environment variables to print.
 */
void ft_env(char **my_envp)
{
	int	i;

	i = 0;
	if (!my_envp)
	{
		//ft_stderror(TRUE, "envp");
		//ft_exit_status(1, TRUE, FALSE);
		//ft_error_handler(); //no array of enviroment variables
		return ; //print nothing
	}
	while (my_envp[i])
	{
		printf("%s\n", my_envp[i]);
		i++;
	}
	ft_exit_status(0, TRUE, FALSE);
}

/* int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	ft_env(envp);
	return (0);
} */
