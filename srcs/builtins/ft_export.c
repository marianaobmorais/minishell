#include "../../includes/minishell.h"

extern char	**environ;

static int	replace_env(char *str, size_t size)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(str, environ[i], size) == 0)
		{
			free(environ[i]);
			environ[i] = str;
			return (0);
		}
		i++;
	}
	return (i);
}

int	ft_export(char *str)
{
	size_t	s_key;
	size_t	i;
	char	**new_environ;

	if (!str || !ft_strchr(str, '='))
		return (-1);
	s_key = (ft_strlen(str) - ft_strlen(ft_strchr(str, '='))) + 1;
	i = replace_env(str, s_key);
	if (i == 0)
		return (0);
	new_environ = (char **) malloc((i + 2) * sizeof(char *));
	if (!new_environ)
		return (-1);
	i = 0;
	while (environ[i])
	{
		new_environ[i] = environ[i];
		i++;
	}
	new_environ[i] = str;
	new_environ[i + 1] = NULL;
	environ = new_environ;
	return (0);
}
