#include "../includes/minishell.h"

static char	**create_env(char **envp)
{
	char	**my_envp;
	int		i;

	i = 0;
	while (envp[i])	
		i++;
	my_envp = (char **) malloc((i + 1) * sizeof(char *));
	if (!my_envp)
		return (NULL);// tratar error
	i = 0;
	while (envp[i])
	{
		my_envp[i] = ft_strdup(envp[i]);
		// fazer check e tratamento de erro;
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}

void	print_env(char **my_envp)
{
	int i = 0;
	while (my_envp[i])
	{
		printf("%s\n", my_envp[i]);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char **my_envp;

	(void)argc;
	(void)argv;
	if (argc > 1)
		return (1);// tratar error
	my_envp = create_env(envp);
	if (!my_envp)
	{
		printf("error\n");
		return (1);
	}

	printf("Oi, eu sou o bashinho\n");

	//print_env(my_envp);

	char *arguments[] = {"export", "_VAR=PRIMEIRA COISA"};
	ft_export(2, arguments, &my_envp);

	print_env(my_envp);

	// char *arguments2[] = {"export", "VAR=                                      SEGUNDA COISA"};
	// ft_export(2, arguments2, &my_envp);

	// print_env(my_envp);

	// char *arguments3[] = {"unset", "VAR"};
	// ft_unset(2, arguments3, &my_envp);

	// print_env(my_envp);

	int i = 0;
	while (my_envp[i])
	{
		free(my_envp[i]);
		i++;
	}
	free(my_envp);
	return (0);
}
