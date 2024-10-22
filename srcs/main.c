#include "../includes/minishell.h"

void	ft_free_vector(char **vector)
{
	int	i;

	i = 0;
	if (vector)
	{
		while (vector[i])
		{
			free(vector[i]);
			i++;
		}
	}
	free(vector);
}
//para testar o echo
/* int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	ft_echo("-nnnnnnnnnnnnnnnnnnnn -nn -no -n oi   'oi'oi'' \"tchau\"");
	return (0);
} */


int	main(int argc, char **envp)
{
	char	**my_envp;
	char	*input;

	if (argc != 1)
	{
		printf("minishell: arguments are not accepted when initializing the program\n"); //ft_error_handler();
		return (1); // conferir numero de saida
	}
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
	{
		ft_error_handler();
		return (1); // conferir numero de saida
	}
	while (1)
	{
		input = readline(PROMPT);
		printf("esse eh o input: %s\n", input);
	}
	ft_free_vector(my_envp);
	return (0);
}


//pra testar cd
/* int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char **my_envp;

	// if (argc != 1)
	// {
	// 	ft_error_handler();
	// 	return (1); // conferir numero de saida
	// }
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
		return (1);

	int	i = 0;
	while (my_envp[i])
	{
		printf("%s\n", my_envp[i]);
		i++;
	}

	ft_cd(argc, argv[1], my_envp);

	printf("\n\n");
	i = 0;
	while (my_envp[i])
	{
		printf("%s\n", my_envp[i]);
		i++;
	}

	ft_free_vector(my_envp);
	return (0);
} */


