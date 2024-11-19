#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char	**my_envp;
	char	*error_msg;

	error_msg = "arguments are not accepted when initializing the program";
	if (argc != 1)
	{
		perror("");
		return (ft_stderror(error_msg), 0);
	}
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
	{
		ft_stderror(""); //corrigir para buscar erro do stderr
		ft_error_handler();
		return (1); //conferir numero de saida
	}
	ft_cli(my_envp);
	ft_free_vector(my_envp);
	return (0);
}
