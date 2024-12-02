#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	t_env	*env;
	char	*error_msg;
	char	**my_envp;

	error_msg = "arguments are not accepted when initializing the program";
	env = (t_env *) malloc(sizeof(t_env));
	// env->global = NULL;
	// env->local = NULL;
	if (argc != 1)
		return (ft_stderror(FALSE, error_msg), 1);
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
		return (ft_stderror(TRUE, ""), 1);
	env->global = my_envp;
	ft_cli(env);
	ft_free_vector(env->global);
	//ft_free_vector(env->local);
	return (0);
}
