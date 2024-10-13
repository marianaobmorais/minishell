#include "../../includes/minishell.h"

extern char	**environ;

static int	exist_env(char *str, size_t size)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (environ[i])
	{
		if (ft_strncmp(str, environ[i], size) == 0 && environ[i][size] == '=')
			j++;
		i++;
	}
	if (j > 0)
		return (i);
	else
		return (0);
}

int	ft_unset(char *str)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	**new_environ;

	if (!str)
		return (-1);
	size = ft_strlen(str);
	i = exist_env(str, size);
	if (i == 0)
		return (0);
	new_environ = (char **) malloc(i * sizeof(char *));
	if (!new_environ)
		return (-1);
	i = 0;
	j = 0;
	while (i > size)
	{
		if (ft_strncmp(str, environ[i], size) == 0 && environ[i][size] == '=')
			free(environ[i++]);
		new_environ[j++] = environ[i++];
	}
	new_environ[j] = NULL;
	free(environ);
	environ = new_environ;
	return (0);
}
