#include "../../includes/minishell.h"

static int check_key(char **argv)
{
	size_t i;

	while (*argv)
	{
		i = 0;
		if (!ft_isalpha((*argv)[i]) && (*argv)[i] != '_')
			return (ft_stderror(2, "export: '%s' not a valid identifier\n", (*argv)), -1);
		while ((*argv)[i] != '=')
		{
			if (!ft_isalnum((*argv)[i]) && (*argv)[i] != '_')
			{
				if ((*argv)[i] != '+')
					return (printf("export: '%s' not a valid identifier\n", (*argv))); // not a valid identifier
				else if ((*argv)[i] == '+' && (*argv)[i + 1] != '=')
					return (printf("export: '%s' not a valid identifier\n", (*argv))); // not a valid identifier
			}
			i++;
		}
		argv++;
	}
	return (0);
}

static void	add_var(char *str, size_t size, char ***my_envp)
{
	int i;
	char **new_envp;

	i = 0;
	new_envp = (char **) malloc((size + 2) * sizeof(char *));
	if (!new_envp)
		exit(1); // tratamento de erro
	i = 0;
	while ((*my_envp)[i])
	{
		new_envp[i] = (*my_envp)[i];
		i++;
	}
	new_envp[i] = ft_strdup(str); // tratatamento de erro
	new_envp[i + 1] = NULL;
	free((*my_envp));
	*my_envp = new_envp;
}

static int replace_var(char *str, size_t size, char ***my_envp)
{
	int i;

	i = 0;
	while ((*my_envp)[i])
	{
		if (ft_strncmp(str, (*my_envp)[i], size) == 0
			&& (*my_envp)[i][size] == '=')
		{
			free((*my_envp)[i]);
			(*my_envp)[i] = ft_strdup(str);
			return (0);
		}
		i++;
	}
	add_var(str, i, my_envp);
	return (i);
}

static int concatenate_var(char *str, char ***my_envp)
{
	int		i;
	int		size;
	char	*temp_str;
	char	*value;

	i = 0;
	while ((*my_envp)[i])
	{
		value = ft_strchr(str, '+') + 2;
		size = (ft_strlen(str) - ft_strlen(value)) - 2;
		if (ft_strncmp(str, (*my_envp)[i], size) == 0
			&& (*my_envp)[i][size] == '=')
		{
			temp_str = ft_strjoin((*my_envp)[i], value);
			free((*my_envp)[i]);
			(*my_envp)[i] = temp_str;
			return (0);
		}
		i++;
	}
	add_var(str, i, my_envp);
	return (i);
}

int	ft_export(int argc, char **argv, char ***my_envp)
{
	size_t s_key;

	if (argc == 1)
	{
		ft_print_export(*my_envp);
		return (ft_exit_status(0, TRUE, TRUE));
	}
	argv++;
	if (check_key(argv) != 0)
		return (ft_exit_status(2, TRUE, TRUE));
	while (*argv)
	{
		if (!ft_strchr(*argv, '='))
			return (ft_exit_status(0, TRUE, TRUE));
		// nao fazer nada e guardar junto com as variaveis que foram exportadas
		s_key = (ft_strlen(*argv) - ft_strlen(ft_strchr(*argv, '=')));
		if ((*argv)[s_key - 1] == '+')
			concatenate_var(*argv, my_envp);
		else
			replace_var(*argv, s_key, my_envp);
		argv++;
	}
	return (ft_exit_status(0, TRUE, TRUE));
}
