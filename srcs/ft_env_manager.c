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
/* 
void	ft_export_local(void *node)
{
	t_exec	*execnode;

	if ()

} */

char	**ft_merge_env(t_shell *sh)
{
	int		global_size;
	int		local_size;
	int		i;
	int		z;
	char	**envp;

	global_size = ft_argslen(sh->global);
	local_size = ft_argslen(sh->local);
	z = 0;
	i = 0;
	envp = (char **) malloc((global_size + local_size + 1) * sizeof(char *));
	if (!envp)
	{
		ft_stderror(TRUE, "envp");
		ft_exit_status(1, TRUE, FALSE);
	}
	while((sh->global)[i])
	{
		envp[i] = ft_strdup((sh->global)[i]);
		i++;
	}
	if (sh->local)
	{
		while((sh->local)[z])
		{
			envp[i] = ft_strdup((sh->local)[z]);
			i++;
			z++;
		}
	}
	envp[i] = NULL;
	return (envp);
}

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
