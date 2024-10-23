#include "../../includes/minishell.h"

static size_t	exist_venv(char *str, size_t size, char ***my_envp)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while ((*my_envp)[i])
	{
		if (ft_strncmp(str, (*my_envp)[i], size) == 0 && (*my_envp)[i][size] == '=')
			j++;
		i++;
	}
	if (j > 0)
		return (i);
	else
		return (0);
}

int	ft_unset(int argc, char **argv, char ***my_envp)
{
	size_t	i;
	size_t	j;
	size_t	size;
	size_t	size_env;
	char	**new_environ;

	if (argc != 2)
		return (-1);//tratar error
	if (!argv[1])
		return (-1);//tratar error
	size = ft_strlen(argv[1]);
	size_env = exist_venv(argv[1], size, my_envp);
	if (size_env == 0)
		return (0);
	new_environ = (char **) malloc(size_env * sizeof(char *));
	if (!new_environ)
		return (-1);
	i = 0;
	j = 0;
	while (i < size_env)
	{
		if (ft_strncmp(argv[1], (*my_envp)[i], size) == 0 && (*my_envp)[i][size] == '=')
			free((*my_envp)[i++]);
		new_environ[j++] = (*my_envp)[i++];
	}
	//new_environ[j] = NULL;
	free((*my_envp));
	*my_envp = new_environ;
	return (0);
}
