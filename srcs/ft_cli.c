#include "../includes/minishell.h"

void	ft_history(char *input)
{
	// int	i;
	// int	w;

	if (input)
		add_history(input);
}

void	ft_cli(void)
{
	char	*input;
	pid_t pid;

	ft_signal_parent();
	while (1)
	{
		input = readline(PROMPT);
		if (input)
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
		if (!input)
		{
			ft_exit(FALSE);
			break;
		}
	}
}
