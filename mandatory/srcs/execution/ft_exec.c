/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:31:57 by joneves-          #+#    #+#             */
/*   Updated: 2025/01/02 18:39:05 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Safely allocates memory for an array of strings.
 *
 * This function allocates memory for an array of strings, ensuring that the
 * allocation is successful. If the allocation fails, the `ft_error_malloc`
 * function is called to handle the error.
 *
 * @return A pointer to the allocated array of strings.
 */
static char	**safe_malloc_str(void)
{
	char	**n_args;

	n_args = (char **)malloc(sizeof(char *));
	if (!n_args)
		ft_error_malloc("new_args");
	*n_args = NULL;
	return (n_args);
}

/**
 * @brief Converts a list of tokens to an array of strings.
 *
 * Iterates through a linked list of tokens, converting each token's value
 * to a string and storing it in a newly allocated array. Handles memory
 * allocation and error checking.
 *
 * @param args A double pointer to the list of tokens.
 *
 * @return A newly allocated array of strings, or NULL on error.
 */
char	**tokentostring(t_list **args)
{
	char	**n_args;
	char	**new_args_cp;
	t_list	*curr;
	int		i;

	n_args = safe_malloc_str();
	curr = *args;
	while (curr)
	{
		if (((t_token *)(curr)->content)->expand
			&& !((t_token *)(curr)->content)->state)
		{
			new_args_cp = ft_split(((t_token *)(curr)->content)->value, ' ');
			i = -1;
			while (new_args_cp[++i])
				n_args = ft_add_to_vector(n_args, new_args_cp[i]);
			ft_free_vector(new_args_cp);
		}
		else
			n_args = ft_add_to_vector(n_args,
					((t_token *)(curr)->content)->value);
		curr = (curr)->next;
	}
	return (n_args);
}

/**
 * @brief Resolves the pathname for a command using provided paths.
 *
 * This function iterates through a list of paths, concatenates each path with 
 * the command, and checks if the resulting pathname is accessible and valid. 
 * If a valid pathname is found, it frees unused paths and returns the resolved 
 * pathname. If no valid pathname is found, all paths are freed, and `NULL` is 
 * returned.
 *
 * @param paths An array of paths to search for the command.
 * @param cmds An array of command arguments, where `cmds[0]` is the command name
 * @param sh A pointer to the shell structure for additional validation.
 * @return A valid pathname string for the command, or `NULL` if not found.
 */
static char	*ft_handle_pathname(char **paths, char **cmds, t_shell *sh)
{
	char	*pathname;
	int		i;

	i = -1;
	while (paths[++i])
	{
		pathname = merge(merge(paths[i], "/"), cmds[0]);
		if (access(pathname, F_OK) == 0
			&& isvalid_file(pathname, cmds, sh) == 0)
			return (ft_free_paths(paths, i), pathname);
		free(pathname);
	}
	return (free(paths), NULL);
}

/**
 * @brief Finds the full path of a command in the system's PATH.
 *
 * Searches for the executable file of a command in the directories
 * listed in the system's PATH environment variable. It validates
 * the existence and executability of the command.
 *
 * @param envp An array of strings representing the environment variables.
 * @param cmds An array of strings representing the command and its arguments.
 *
 * @return A newly allocated string containing the full path of the command, 
 *         or NULL if the command is not found.
 */
static char	*ft_findpath(char **envp, char **cmds, t_shell *sh)
{
	char	**paths;
	char	*pathname;
	int		i;

	i = 0;
	paths = NULL;
	if (!*cmds[0])
		return (NULL);
	if (ft_strchr(cmds[0], '/') && isvalid_dir(cmds[0], cmds, sh) == 0)
		return (ft_strdup(cmds[0]));
	if (access(cmds[0], F_OK) == 0 && isvalid_file(cmds[0], cmds, sh) == 0)
		return (ft_strdup(cmds[0]));
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (envp[i])
		paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	pathname = ft_handle_pathname(paths, cmds, sh);
	return (pathname);
}

/**
 * @brief Executes a command in the shell.
 *
 * Processes the list of args, checks if the command is a built-in function,
 * and executes it. If not, it finds the path of the command and executes it
 * using execve. Handles errors and sets the appropriate exit status.
 *
 * @param args A double pointer to the list of arguments.
 * @param sh The shell structure containing environment variables and settings.
 */
int	ft_exec(t_list **args, t_shell *sh)
{
	char	*pathname;
	char	**new_args;

	pathname = NULL;
	ft_process_token_list(args, ft_merge_env(sh->global, sh->local));
	new_args = tokentostring(args);
	if (ft_isbuiltin(new_args) && ft_exit_status(0, FALSE, FALSE) != 141)
		ft_exec_builtin(new_args, sh);
	else
	{
		if (!*new_args)
			return (ft_child_cleaner(sh, new_args, 0), \
				ft_exit_status(0, TRUE, TRUE));
		pathname = ft_findpath(sh->global, new_args, sh);
		if (!pathname)
			return (ft_stderror(FALSE, "%s: command not found", new_args[0]), \
				ft_child_cleaner(sh, new_args, 0), \
				ft_exit_status(127, TRUE, TRUE));
		if (execve(pathname, new_args, sh->global) == -1)
			return (ft_stderror(TRUE, "%s: ", new_args[0]), \
				ft_child_cleaner(sh, new_args, 0), free(pathname), \
				ft_exit_status(1, TRUE, TRUE));
		free(pathname);
	}
	return (ft_child_cleaner(sh, new_args, 0), ft_exit_status(0, FALSE, TRUE));
}
