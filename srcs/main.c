#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	t_env	*env;
	char	*error_msg;

	error_msg = "arguments are not accepted when initializing the program";
	env = (t_env *) malloc(sizeof(t_env));
	env->global = NULL;
	env->local = NULL;
	if (argc != 1)
		return (ft_stderror(FALSE, error_msg), 1);
	env->global = ft_get_my_envp(envp);
	if (!env->global)
		return (ft_stderror(TRUE, ""), 1);
	ft_cli(env);
	ft_free_vector(env->global);
	return (0);
}
