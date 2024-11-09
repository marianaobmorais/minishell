#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char	**my_envp;
	char	*error_msg;

	error_msg = "arguments are not accepted when initializing the program\n";
	if (argc != 1)
	{
		return (ft_stderror(128, error_msg));
	}
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
	{
		ft_stderror(1, "");
		ft_error_handler();
		return (1); // conferir numero de saida
	}
	ft_cli(&my_envp);
	ft_free_vector(my_envp);
	return (0);
}
