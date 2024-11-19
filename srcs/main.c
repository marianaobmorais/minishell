#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char	**my_envp;
	char	*error_msg;

	error_msg = "arguments are not accepted when initializing the program";
	if (argc != 1)
		return (ft_stderror(FALSE, error_msg), 1);
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
		return (ft_stderror(TRUE, ""), 1);
	ft_cli(&my_envp);
	ft_free_vector(my_envp);
	return (0);
}
