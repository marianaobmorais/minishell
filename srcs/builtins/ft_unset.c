#include "../../includes/minishell.h"

static size_t	is_var(char *str, char ***my_envp)
{
	size_t	index_envp;
	size_t	flag;
	size_t	size_var;

	flag = 0;
	index_envp = 0;
	size_var = ft_strlen(str);
	while ((*my_envp)[index_envp])
	{
		if (ft_strncmp(str, (*my_envp)[index_envp], size_var) == 0
			&& (*my_envp)[index_envp][size_var] == '=')
			flag++;
		index_envp++;
	}
	if (flag > 0)
		return (index_envp);
	else
		return (0);
}

static int	delete_var(char *str, char ***my_envp, size_t size_env)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	**new_environ;

	size = ft_strlen(str);
	i = 0;
	j = 0;
	new_environ = (char **) malloc(size_env * sizeof(char *));
	if (!new_environ)
		return (-1); //tratar erro
	while (i < size_env)
	{
		if (ft_strncmp(str, (*my_envp)[i], size) == 0
			&& (*my_envp)[i][size] == '=')
		{
			free((*my_envp)[i]);
			i++;
		}
		new_environ[j++] = (*my_envp)[i++];
	}
	new_environ[j] = NULL;
	free((*my_envp));
	*my_envp = new_environ;
	return (0);
}

int	ft_unset(int argc, char **argv, char ***my_envp)
{
	size_t	size_env;

	if (argc == 1)
		return (0); //nao fazer nada
	++argv;
	while (*argv)
	{
		size_env = is_var(*argv, my_envp);
		if (size_env > 0)
			delete_var(*argv, my_envp, size_env); // tem que delatar de envp_history do export, apago os que nao foram exportados
		argv++;
	}
	return (0);
}
