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
void ft_env(int argc, char **args, char **my_envp)
{
	int	i;

	i = 0;
	if (!my_envp)
	{
		ft_exit_status(0, TRUE, FALSE);
		return ;
	}
	if (argc > 1)
	{
		ft_stderror(FALSE, "env: `%s`: No such file or directory", args[1]);
		ft_exit_status(127, TRUE, FALSE);
		return ;
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
