#include "../includes/minishell_bonus.h"

/**
 * @brief Restores the CLI state within the shell structure.
 *
 * This function resets the shell's state by clearing saved file descriptors, 
 * setting the run flag to TRUE, and resetting the previous command pointer. 
 * It also frees the existing heredoc list and allocates a new one. 
 * If allocation fails, an error message is displayed.
 *
 * @param sh Pointer to the t_shell structure.
 * @param tree Unused parameter, reserved for future use.
 */
void	ft_restore_cli(t_shell *sh)
{
	sh->fds_saved = 0;
	sh->run = TRUE;
	sh->search_heredoc = FALSE;
	sh->prev = NULL;
	if (sh->heredoc_list)
	{
		ft_lstclear(sh->heredoc_list, free);
		free(sh->heredoc_list);
	}
	sh->heredoc_list = (t_list **) malloc(sizeof(t_list **));
	if (!sh->heredoc_list)
		ft_stderror(TRUE, "");
	*(sh->heredoc_list) = NULL;
	sh->error_fd = 0;
	if (sh->root)
	{
		ft_free_tree(sh->root);
		sh->root = NULL;
	}
	close_original_fds(sh);
}

/**
 * @brief Initializes the variables in the shell structure.
 *
 * This function sets the initial values for the variables in the sh structure.
 *
 * @param sh A pointer to the shell structure to be initialized.
 */
void	ft_init_var_sh(t_shell *sh)
{
	sh->limbo[0] = NULL;
	sh->local[0] = NULL;
	sh->heredoc_list[0] = NULL;
	sh->fds_saved = 0;
	sh->error_fd = 0;
	sh->run = TRUE;
	sh->search_heredoc = FALSE;
	sh->sub_root = FALSE;
	sh->stdin_ = -1;
	sh->stdout_ = -1;
	sh->stderr_ = -1;
	sh->prev = NULL;
	sh->root = NULL;
	sh->fds[0] = -1;
	sh->fds[1] = -1;
}

/**
 * @brief Initializes a shell structure with environment variables.
 *
 * This function allocates memory for a t_shell structure and its members.
 * It initializes environment variables, local variables, heredoc list, and
 * other fields. If any allocation fails, it frees the allocated memory and
 * returns NULL with an appropriate error message.
 *
 * @param envp Array of environment variables.
 * @return t_shell* Pointer to the initialized t_shell structure, or NULL if 
 * any allocation fails.
 */
t_shell	*ft_init_sh(char **envp)
{
	t_shell	*sh;

	sh = (t_shell *)malloc(sizeof(t_shell));
	if (!sh)
		return (ft_error_malloc("sh"), NULL);
	sh->global = ft_get_my_envp(envp);
	if (!sh->global)
		return (ft_error_malloc("sh->global"), NULL);
	sh->local = (char **) malloc(sizeof(char *));
	if (!sh->local)
		return (ft_free_vector(sh->global), free(sh), \
			ft_error_malloc("sh->local"), NULL);
	sh->limbo = (char **) malloc(sizeof(char *));
	if (!sh->limbo)
		return (ft_free_vector(sh->global), ft_free_vector(sh->local), \
			free(sh), ft_error_malloc("sh->limbo"), NULL);
	sh->heredoc_list = (t_list **)malloc(sizeof(t_list *));
	if (!sh->heredoc_list)
	{
		ft_free_vector(sh->local);
		ft_free_vector(sh->global);
		return (free(sh), ft_error_malloc("sh->heredoc_list"), NULL);
	}
	ft_init_var_sh(sh);
	return (sh);
}

/**
 * @brief Adds a cmd to the history if it contains non-whitespace characters.
 *
 * This function checks the input cmd string to determine if it consists only 
 * of whitespace characters. If the cmd contains any non-whitespace characters,
 * it is added to the cmd history. If the input is entirely whitespace, it is 
 * ignored.
 *
 * @param input The cmd string to be checked and potentially added to history.
 * @return int Returns 1 if the cmdwas added to history, or 0 if it was ignored
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
 * This function initializes and enters an infinite loop where it prompts the
 * user for input, processes the input, and handles command execution. It sets
 * up signal handling, reads user input from the prompt, and checks for valid
 * commands to add to history and execute. If the input is empty (EOF), it 
 * exits the program.
 *
 * @param my_envp A pointer to the array of environment variables, passed to
 *        functions that execute commands with the current environment.
 */
void	ft_cli(t_shell *sh)
{
	char	*input;

	input = NULL;
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
			return (free(input), ft_putstr_fd("exit\n", 1), rl_clear_history());
		if (ft_history(input))
		{
			sh->root = ft_process_input(input);
			if (sh->root)
				ft_launcher_manager(sh->root, sh);
			ft_restore_cli(sh);
		}
	}
	rl_clear_history();
}
