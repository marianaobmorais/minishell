#include "../includes/minishell.h"

void	ft_restore_cli(t_shell *sh, void **tree)
{
	(void)tree;
	//free

	sh->fds_saved = 0;
	sh->run = TRUE;
	sh->prev = NULL;
	free(sh->heredoc_list);
	sh->heredoc_list = (t_list **) malloc(sizeof(t_list **));
	if (!sh->heredoc_list)
		ft_stderror(TRUE, "");
	*(sh->heredoc_list) = NULL;
	//ft_free_tree(tree);
}

t_shell	*ft_init_sh(char **envp)
{
	t_shell	*sh;
	char	**my_envp;

	sh = (t_shell *) malloc(sizeof(t_shell));
	if (!sh)
		return (ft_stderror(TRUE, ""), NULL);
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
		return (ft_stderror(TRUE, ""), NULL);
	sh->global = my_envp;
	sh->local = NULL;
	sh->fds_saved = 0;
	sh->run = TRUE;
	sh->prev = NULL;
	sh->heredoc_list = (t_list **) malloc(sizeof(t_list **));
	if (!sh->heredoc_list)
	{
		free(sh);
		return (ft_stderror(TRUE, ""), NULL);
	}
	*(sh->heredoc_list) = NULL;
	return (sh);
}

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
void	ft_cli(t_shell *sh)
{
	char	*input;
	void	**tree;

	input = NULL;
	tree = NULL;
	while (1)
	{
		ft_signal(PARENT_);
		if (input)
		{
			free(input);
			input = NULL;
		}
		input = readline(PROMPT);
		if (!input)
		{
			free(input);
			ft_putstr_fd("exit\n", 2);
			break ;
		}
		if (ft_history(input))
		{
			tree = ft_process_input(input, sh->global);
			if (tree)
			{
				ft_search_heredoc(tree, sh);
				if (sh->run == TRUE && !ft_single_command(tree, sh))
					ft_launcher(tree, ((t_pipe *)tree)->right, NULL, sh);
			}
			ft_restore_cli(sh, tree);
		}
	}
	rl_clear_history();
}
