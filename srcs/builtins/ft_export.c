#include "../../includes/minishell.h"

static int	check_key(char *str, size_t size)
{
	size_t i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
	{
		printf("export: '%s' not a valid identifier\n", str);
		return (-1); //not a valid identifier
	}
	while (i < size)
	{
		if ((!ft_isalnum(str[i]) && str[i] != '_') && (str[i] == '+'
			&& str[i + 1] != '='))
		{
			printf("[%c]export: '%s' not a valid identifier\n",str[size], str);
			return (-1); //not a valid identifier			
		}
		i++;
	}
	return (0);
}

static int	replace_var(char *str, size_t size, char ***my_envp)
{
	int	i;

	i = 0;
	while ((*my_envp)[i])
	{
		if (ft_strncmp(str, (*my_envp)[i], size - 1) == 0)
		{
			free((*my_envp)[i]);
			(*my_envp)[i] = ft_strdup(str);
			return (0);
		}
		i++;
	}
	return (i);
}

// static int	concatenate_var(char *str, size_t size, char ***my_envp)
// {
// 	int	i;

// 	while ((*my_envp)[i])
// 	{
// 		if (ft_strncmp(str, (*my_envp)[i], size) == 0)
// 		{
// 			free((*my_envp)[i]);
// 			(*my_envp)[i] = ft_strdup(str);
// 			return (0);
// 		}
// 		i++;
// 	}
// 	return (i);
// }

int	ft_export(int argc, char **argv, char ***my_envp)
{
	size_t	s_key;
	size_t	i;
	char	**new_envp;

	//se tiver mais de uma eh para exportar tbm
	if (argc == 1)
		return (-1); //printf das variaveis que foram exportadas
	if (!ft_strchr(argv[1], '='))
		return (-1); //nao fazer nada e guardar junto com as variaveis que foram exportadas
	s_key = (ft_strlen(argv[1]) - ft_strlen(ft_strchr(argv[1], '=')));
	if (check_key(argv[1], s_key) == -1)
		exit(1);
	//chamar de forma recursiva export para os demais argvs
	//check se e concatenate
	printf("%c\n", argv[1][s_key]);
	if (argv[1][s_key - 1] == '+')
		printf("CONCATENAR\n");
	i = replace_var(argv[1], s_key, my_envp); //ter funcao de check e fazer else de append ou replace, ao final de cada um se nao achar fazer funcao de add
	if (i == 0)
		return (0);
	new_envp = (char **) malloc((i + 2) * sizeof(char *));
	if (!new_envp)
		return (-1);
	i = 0;
	while ((*my_envp)[i])
	{
		new_envp[i] = (*my_envp)[i];
		i++;
	}
	new_envp[i] = ft_strdup(argv[1]);
	new_envp[i + 1] = NULL;
	free((*my_envp));
	*my_envp = new_envp;
	return (0);
}
