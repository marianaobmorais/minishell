#include "../../includes/minishell.h"

void ft_env(char **my_envp)
{
	int	i;

	i = 0;
	if (!my_envp)
		ft_error_handler();
	while (my_envp[i])
	{
		printf("%s\n", my_envp[i]);
		i++;
	}
}

/* int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	ft_env(envp);
	return (0);
} */
