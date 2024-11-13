#include "../includes/minishell.h"

/**
 * @brief Adds a command to the history if it contains non-whitespace characters.
 *
 * This function checks the input command string to determine if it consists only 
 * of whitespace characters. If the command contains any non-whitespace characters, 
 * it is added to the command history. If the input is entirely whitespace, it is 
 * ignored.
 *
 * @param input The command string to be checked and potentially added to history.
 * @return int Returns 1 if the command was added to history, or 0 if it was ignored.
 */
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
	//heredoc_fd("here");
	if (strcmp(input, "child") == 0) //test
	{
		count_line(0);// resolver com struct para persistir qtde linhas inseridas no heredoc
		pid = fork();
		if (pid == 0)
		{
			ft_signal(DEFAULT);
			heredoc_fd("here");
			char *algo[] = {"/usr/bin/cat", NULL};
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
		ft_stderror("Exit Status: %d", ft_exit_status(0, FALSE, FALSE));
	}
}

/**
 * @brief Main command-line interface loop for processing user commands.
 *
 * This function initializes and enters an infinite loop where it prompts the user 
 * for input, processes the input, and handles command execution. It sets up signal 
 * handling, reads user input from the prompt, and checks for valid commands to add 
 * to history and execute. If the input is empty (EOF), it exits the program.
 *
 * @param my_envp A pointer to the array of environment variables, passed to functions 
 *                that execute commands with the current environment.
 */
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
		input = readline("$ ");
		if (!input)
		{
			free(input);
			ft_exit(FALSE, 0);
			break;
		}
		if (ft_history(input))
			ft_launcher(input, my_envp);
	}
	rl_clear_history();
}
