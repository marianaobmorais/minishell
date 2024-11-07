#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char	**my_envp;

	if (argc != 1)
	{
		printf("minishell: arguments are not accepted when initializing the program\n"); //ft_error_handler();
		return (1); // conferir numero de saida
	}
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
	{
		ft_error_handler();
		return (1); // conferir numero de saida
	}
	ft_cli(&my_envp);
	ft_free_vector(my_envp);
	return (0);
}