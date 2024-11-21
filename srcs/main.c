#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	t_env	env;
	char	*error_msg;
	char	**my_envp;

	//verificar necessidade de manipular SHLVL
	error_msg = "arguments are not accepted when initializing the program";
	env.global = NULL;
	env.local = NULL;
	if (argc != 1)
		return (ft_stderror(FALSE, error_msg), 1);
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
		return (ft_stderror(TRUE, ""), 1);
	env.global = &my_envp;
	ft_cli(&env);
	ft_free_vector(*(env.global));
	return (0);
}
