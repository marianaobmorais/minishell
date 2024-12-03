#include "../includes/minishell.h"

// void	ft_local_variable()
// {

// }

// t_env	*ft_init_env(char **envp)
// {
// 	t_env	*env;
// 	char	**my_envp;

// 	env = (t_env *) malloc(sizeof(t_env));
// 	my_envp = ft_get_my_envp(envp);
// 	if (!my_envp)
// 		return (ft_stderror(TRUE, ""), NULL);
// 	env->global = my_envp;
// 	env->local = NULL;
// 	return (env);
// }

char	**ft_get_my_envp(char **envp)
{
	char	**my_envp;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	my_envp = (char **)malloc(sizeof(char **) * (i + 1));
	if (!my_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		my_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}
