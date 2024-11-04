#include "../includes/minishell.h"

/* int	main(int argc, char **argv, char **envp)
{
	(void)argv;
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
		if (input)
		{
			//ft_process_input(input, my_envp);
		}
		else if (!input)
		{
			ft_exit(FALSE); // end EOF
			break;
		}
	}
	ft_free_vector(my_envp);
	return (0);
} */

void	handler_test(int sig)
{
	(void)sig;
	printf("Aborded\n");
	rl_on_new_line();
	rl_redisplay();
}

void	sigcontrol(void)
{
	struct sigaction sa;

	sa.sa_handler = &handler_test;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	sigaction(SIGCHLD, &sa, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char	**my_envp;
	char	*input;

	//sigcontrol();
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
		{
			//ft_process_input(input, my_envp);
			if (strcmp(input, "bad") == 0)
			{
				pid_t pid;
				pid = fork();
				if (pid == 0)
				{
					char *q[] = {"/home/jose/minishell_42/bad", NULL}; //com erro
					//char *q[] = {"/usr/bin/ls", NULL}; //com sem erro
					if (execv(q[0], q) == -1)
						printf("error\n");
				}
			}
			wait(NULL);
		}
		else if (!input)
		{
			ft_exit(FALSE); // end EOF
			break;
		}
	}
	ft_free_vector(my_envp);
	return (0);
}