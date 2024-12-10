#include "../../includes/minishell.h"

static int	delete_var_limbo(char *str, char ***my_envp, size_t size_env)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	**new_envp;

	size = ft_strlen(str);
	i = 0;
	j = 0;
	new_envp = (char **) malloc((size_env + 1) * sizeof(char *));
	if (!new_envp)
		return (ft_exit_status(1, TRUE, FALSE), -1); //utilizar funcao malloc
	while (i < size_env)
	{
		if (ft_strncmp(str, (*my_envp)[i], size) == 0)
		{
			free((*my_envp)[i]);
			i++;
		}
		new_envp[j++] = (*my_envp)[i++];
	}
	new_envp[j] = NULL;
	free((*my_envp));
	*my_envp = new_envp;
	return (0);
}

int	ft_limbo_import(t_shell *sh, char *arg)
{
	int		i;
	int		size;
	char	*var;

	i = 0;
	var = NULL;
	while ((sh->limbo)[i])
	{
		size = ft_strlen((sh->limbo)[i]);
		if (ft_strncmp(arg, (sh->limbo)[i], size) == 0
			&& arg[size] == '=')
		{
			var = (sh->limbo)[i];
			break ;
		}
		i++;
	}
	if (var)
	{
		add_var(arg, ft_argslen(sh->global), &(sh->global));
		delete_var_limbo(var, &(sh->limbo), ft_argslen((sh->limbo)));
		return (0);
	}
	return (-1);
}
