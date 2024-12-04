#include "../includes/minishell.h"

char	**ft_merge_env(t_shell *sh)
{
	int		size;
	int		i;
	int		z;
	char	**envp;

	z = 0;
	i = 0;
	size = ft_argslen(sh->local) + ft_argslen(sh->global);
	envp = (char **) malloc((size + 1) * sizeof(char *));
	if (!envp)
		return (ft_exit_status(1, TRUE, FALSE), ft_stderror(TRUE, ""), NULL);
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
	my_envp = (char **)malloc(sizeof(char *) * (i + 1));
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
