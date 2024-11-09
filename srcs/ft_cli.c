#include "../includes/minishell.h"

int	ft_history(char *input)
{
	int	i;
	int	w;

	i = 0;
	w = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			w++;
		i++;
	}
	if (i != w)
	{
		add_history(input);
		return (1);
	}
	return (0);	
}

void	ft_launcher(char *input, char ***my_envp)
{
	pid_t	pid;
	int		status;

	ft_process_input(input, *my_envp);
	if (strcmp(input, "child") == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			ft_signal(DEFAULT);
			char *algo[] = {"/usr/bin/sleep", "50", NULL};
			execve(algo[0], algo, *my_envp);
		}
		else
		{
			if (waitpid(pid, &status, 0) != -1)
			{
				if (WIFEXITED(status))
					ft_exit_status(WEXITSTATUS(status), TRUE, FALSE); //grava exit status
        		else if (WIFSIGNALED(status))
				{
					ft_signal(CHILD);
					ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE); //grava exit status
				}
			}
		}
	}
	if (strcmp(input, "status") == 0) //Test
	{
		ft_stderror(0, "Qualquer coisa: ");
		printf("%d\n", ft_exit_status(0, FALSE, FALSE));
	}
}

void	ft_cli(char ***my_envp)
{
	char	*input;

	input = NULL;
	ft_signal(PARENT);
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
		if (ft_history(input))
		{
			ft_launcher(input, my_envp);
		}
	}
	rl_clear_history();
}
