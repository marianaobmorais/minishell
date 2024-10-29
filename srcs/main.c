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

/* char	*ft_tokenize_input(char *input)
{
	char	*token;
	t_token	*node;

	while (*input)
	{
		if (ft_strchr(METACHARS, *input))
		{
			node = (t_token *)ft_lstnew(token);
			printf("token = %s\n", token);
		}
		token = ft_charjoin(token, *input);
		input++;
	}
	return (token);
} */

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char	**my_envp;
	char	**args;
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
		if (input)
			args = ft_split_expand(input, ' ', my_envp);
		int i = 0;
		while (args[i])
		{
			printf("token %d: %s\n", i + 1, args[i]);
			i++;
		}
	}
	ft_free_vector(my_envp);
	return (0);
}
