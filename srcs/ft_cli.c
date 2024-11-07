#include "../includes/minishell.h"

void	ft_history(char *input)
{
	int	i;
	int	w;

	i = 0;
	w = 0;
	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
			w++;
		i++;
	}
	if (i != w)
		add_history(input);
}

void	ft_launcher(char *input, char ***envp)
{
	pid_t pid;

	if (strcmp(input, "bad") == 0)
	{
		//printf("Start...");
		pid = fork();
		if (pid == 0)
		{
			ft_signal_child();
			char *algo[] = {"/usr/bin/sleep", "50", NULL};
			execve(algo[0], algo, *envp);
		}
	}
	if (strcmp(input, "status") == 0) //Test
	{
		ft_stderror(0, "Qaulquer coisa");
		printf("%d\n", ft_exit_status(0, FALSE, FALSE));
	}
	wait(0); //capturar status code
}

void	ft_cli(char ***envp)
{
	char	*input;

	input = NULL;
	ft_signal_parent();
	while (1)
	{
		if (input)
		{
			free(input);
			input = NULL;
		}
		input = readline(PROMPT);
		if (!input)
		{
			free(input);
			ft_exit(FALSE);
			break;
		}
		if (input && *input && *input != '\n')
		{
			ft_history(input);
			ft_launcher(input, envp);
		}
	}
	rl_clear_history();
}
