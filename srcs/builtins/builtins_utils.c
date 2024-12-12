#include "../../includes/minishell.h"

/**
 * @brief Counts the number of arguments in an array of strings.
 *
 * This function calculates the length of a null-terminated array of strings.
 * If the array is NULL, the function returns 0.
 *
 * @param args The array of strings to evaluate.
 * @return The number of arguments in the array.
 */
int	ft_argslen(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (i);
	if (!(*args))
		return (i);
	while (args[i])
		i++;
	return (i);
}

/**
 * @brief Checks if the given command is a built-in command.
 *
 * This function compares the provided command (args[0]) against a predefined
 * list of built-in commands (e.g., cd, pwd, export, unset, exit, echo, env).
 * Returns TRUE if the command matches a built-in, otherwise FALSE.
 *
 * @param args The array of arguments, where args[0] is the command name.
 * @return TRUE if args[0] is a built-in command, FALSE otherwise.
 */
bool	ft_isbuiltin(char **args)
{
	const char	*bcmd[8];
	int			i;

	i = 0;
	bcmd[0] = "cd";
	bcmd[1] = "pwd";
	bcmd[2] = "export";
	bcmd[3] = "unset";
	bcmd[4] = "exit";
	bcmd[5] = "echo";
	bcmd[6] = "env";
	bcmd[7] = NULL;
	if (*args)
	{
		while (bcmd[i])
		{
			if (ft_strncmp(bcmd[i], args[0], ft_strlen(args[0])) == 0
				&& *args[0])
				return (TRUE);
			i++;
		}
	}
	return (FALSE);
}

/**
 * @brief Executes a built-in command based on the given arguments.
 *
 * This function identifies and executes a supported built-in command 
 * (e.g., cd, pwd, export, unset, exit, echo, env) using the provided
 * arguments and shell context.
 *
 * @param args The array of arguments, where args[0] is the command name.
 * @param sh The shell structure containing the global and local environments.
 */
void	ft_exec_builtin(char **args, t_shell *sh)
{
	int	argc;

	argc = ft_argslen(args);
	if (ft_strncmp("cd", args[0], ft_strlen(args[0])) == 0)
		ft_cd(argc, args[1], sh->global);
	else if (ft_strncmp("pwd", args[0], ft_strlen(args[0])) == 0)
		ft_pwd();
	else if (ft_strncmp("export", args[0], ft_strlen(args[0])) == 0)
		ft_export(argc, args, sh, GLOBAL);
	else if (ft_strncmp("unset", args[0], ft_strlen(args[0])) == 0)
		ft_unset(argc, args, sh);
	else if (ft_strncmp("exit", args[0], ft_strlen(args[0])) == 0)
		ft_exit(argc, args, sh);
	else if (ft_strncmp("echo", args[0], ft_strlen(args[0])) == 0)
		ft_echo(args);
	else if (ft_strncmp("env", args[0], ft_strlen(args[0])) == 0)
		ft_env(argc, args, sh->global);
}

/**
 * @brief Frees memory allocated for a token list and an argument vector.
 *
 * This function releases memory associated with a token list (`args_cp`) 
 * and a corresponding array of strings (`new_args`).
 *
 * @param args_cp A double pointer to the token list to be freed.
 * @param new_args A double pointer to the array of strings to be freed.
 */
static void	ft_free_helper(t_list **args_cp, char **new_args)
{
	if (*args_cp)
		ft_free_list(*args_cp);
	if (args_cp)
		free(args_cp);
	if (new_args)
		ft_free_vector(new_args);
}

/**
 * @brief Checks if the node contains only a built-in command.
 *
 * This function evaluates a syntax tree node to determine if it represents
 * a built-in command without external commands or pipes. If the node is an
 * executable type and its arguments match a built-in command, the function
 * returns true.
 *
 * @param node The current syntax tree node to check.
 * @param sh The shell structure with environment and execution context.
 * @return
 * - `TRUE` if the node contains a built-in command (e.g., `cd`, `export`, etc.)
 *   without any connected pipes or branching.
 * - `FALSE` otherwise.
 */
int	ft_isjustbuiltin(void *node, t_shell *sh)
{
	t_node	*curr;
	t_list	**args_cp;
	char	**new_args;

	curr = ((t_node *)node)->left;
	if (!((t_node *)node)->right)
	{
		while (curr && ft_is_node_type(curr, REDIR))
			curr = ((t_redir *)curr)->next;
		if (curr && ft_is_node_type(curr, EXEC))
		{
			args_cp = ft_copy_list(((t_exec *)curr)->args);
			ft_process_token_list(args_cp, ft_merge_env(sh->global, sh->local));
			new_args = tokentostring(args_cp);
			if (ft_isbuiltin(new_args) || ((t_exec *)curr)->type == EXPORT
				|| ((t_exec *)curr)->type == EXPORT_AP)
				return (ft_free_helper(args_cp, new_args), TRUE);
			ft_free_helper(args_cp, new_args);
		}
	}
	return (FALSE);
}
