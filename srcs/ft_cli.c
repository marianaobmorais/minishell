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

void	ft_cli(void)
{
	char	*input;
	pid_t pid;

	//ft_signal_parent();
	input = NULL;
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
			break;
		}
		if (input && *input)
		{
			//ft_process_input(input, my_envp);
			ft_history(input);
			if (strcmp(input, "bad") == 0)
			{
				printf("Start...\n");
				pid = fork();
				if (pid == 0)
				{
					ft_signal_child();
					char *algo[] = {"./minishell", NULL};
					execve(algo[0], algo, NULL);
				}
			}
			wait(0);
		}
	}
}
