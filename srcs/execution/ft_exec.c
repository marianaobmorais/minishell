#include "../../includes/minishell.h"

/**
 * @brief Validates the given pathname for execution.
 *
 * Checks if the specified pathname is a valid executable file. It ensures
 * the file exists, is not a directory, and has execution permissions.
 * Logs errors and sets exit status accordingly.
 *
 * @param pathname The path to the file to be validated.
 * @param args An array of arguments for error reporting.
 *
 * @return 0 if valid, or -1 if invalid.
 */
int	isvalid_dir(char *pathname, char **args, t_shell *sh)
{
	struct stat	file;

	if (stat(pathname, &file) == -1) // não sei se preciso fazer o check de *args[0] aqui também
	{
		ft_stderror(TRUE, "stat: %s: ", args[0]);
		ft_child_cleaner(sh, args, 0);
		ft_exit_status(127, TRUE, TRUE);
		return (-1);
	}
	else if (S_ISDIR(file.st_mode) != 0 && *args[0])
	{
		ft_stderror(FALSE, "%s: Is a directory", args[0]);
		ft_child_cleaner(sh, args, 0);
		ft_exit_status(126, TRUE, TRUE);
		return (-1);
	}
	else if (access(pathname, X_OK) == -1 && *args[0])
	{
		ft_stderror(TRUE, "access: %s: ", args[0]);
		ft_child_cleaner(sh, args, 0);
		ft_exit_status(126, TRUE, TRUE);
		return (-1);
	}
	return (0);
}

int	isvalid_(char *pathname, char **args, t_shell *sh)
{
	//write brief
	struct stat	file;

	if (stat(pathname, &file) == -1)
	{
		ft_stderror(TRUE, "stat: %s: ", args[0]);
		ft_child_cleaner(sh, args, 0);
		ft_exit_status(126, TRUE, TRUE);
		return (-1);
	}
	if (S_ISDIR(file.st_mode) != 0)
	{
		return (-1);
	}
	if (access(pathname, R_OK) == -1)
	{
		return (-1);
	}
	if (access(pathname, X_OK) == -1)
	{
		return (-1);
	}
	return (0);
}

/**
 * @brief Merges two strings into a newly allocated string.
 *
 * Concatenates the second string to the first string, frees the memory
 * allocated for the first string, and returns the newly allocated merged
 * string.
 *
 * @param s1 The first string, which will be freed after merging.
 * @param s2 The second string to be concatenated to the first string.
 *
 * @return A newly allocated string containing the merged result.
 */
static char	*merge(char *s1, char *s2)
{
	char	*merge;

	merge = ft_strjoin(s1, s2);
	free(s1);
	s1 = NULL;
	return (merge);
}

/**
 * @brief Frees an array of strings and returns 0.
 *
 * Iterates through an array of strings, freeing each string and then
 * freeing the array itself. Ensures memory is properly deallocated.
 *
 * @param paths The array of strings to be freed.
 * @param i The starting index for freeing the strings.
 *
 * @return Always returns 0.
 */
static int	ft_free_paths(char **paths, int i)
{
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (0);
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
	if (access(cmds[0], F_OK) == 0 && isvalid_(cmds[0], cmds, sh) == 0)
		return (ft_strdup(cmds[0]));
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (envp[i])
		paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		pathname = merge(merge(paths[i], "/"), cmds[0]);
		//if (access(pathname, F_OK) == 0 && isvalid(pathname, cmds) == 0)
		if (access(pathname, F_OK) == 0 && isvalid_dir(pathname, cmds, sh) == 0)
			return (ft_free_paths(paths, i), pathname);
		free(pathname);
		i++;
	}
	free(paths);
	return (NULL);
}


///

/**
 * @brief Adds a new string to a dynamically allocated string vector.
 * 
 * Creates a new vector by appending a given string to an existing vector of strings.
 * Frees the original vector and duplicates its contents into the new one.
 * 
 * @param old_vector Pointer to the original string vector, or NULL if creating a new vector.
 * @param new_str String to add to the vector.
 * @return A pointer to the new vector with the appended string, or NULL if allocation fails.
 */
char	**ft_add_to_vector(char **old_vector, char *new_str)
{
	//review brief
	char	**new_vector;
	int		i;
	i = 0;
	if (old_vector)
		while (old_vector[i])
			i++;
	new_vector = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_vector)
		return (ft_error_malloc("new_vector"), NULL);
	i = 0;
	if (!old_vector)
		new_vector[i++] = ft_strdup(new_str); //malloc check? free allocated mem?
	else
	{
		while (old_vector[i])
		{
			new_vector[i] = ft_strdup(old_vector[i]); //malloc check? free allocated mem?
			i++;
		}
		new_vector[i++] = ft_strdup(new_str); //malloc check? free allocated mem?
		ft_free_vector(old_vector);
	}
	new_vector[i] = NULL;
	return (new_vector);
}



char	**tokentostring_(t_list **args)
{
	//write brief
	char	**new_args;
	char	**new_args_cp;
	t_list	*curr_list;
	t_token	*token;
	int		i;

	ft_print_list(args); //debug
	new_args = (char **)malloc(sizeof(char *));
	if (!new_args)
		ft_error_malloc("new_args");
	curr_list = *args;
	token = ((t_token *)(curr_list)->content);
	while (curr_list)
	{
		if (token->expand)
		{
			new_args_cp = ft_split(token->value, ' ');
			//bashinho [pwd] $ export var="ls -l" var2="-a -w"
			//bashinho [pwd] $ $var $var2 //not working properly
			i = 0;
			while (new_args_cp[i])
			{
				new_args = ft_add_to_vector(new_args, new_args_cp[i]);
				i++;
			}
			ft_free_vector(new_args_cp);
		}
		else
			new_args = ft_add_to_vector(new_args, token->value);
		curr_list = (curr_list)->next;	
	}
	return (new_args);
}

///


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
void	ft_exec(t_list **args, t_shell *sh)
{
	char	*pathname;
	char	**new_args;

	pathname = NULL;
	ft_process_token_list(args, ft_merge_env(sh));
	new_args = tokentostring_(args);
	for (int i = 0; new_args[i]; i++) //debug
		fprintf(stderr, "new_args = %s\n", new_args[i]); //debug
	if (ft_isbuiltin(new_args))
		ft_exec_builtin(new_args, sh);
	else
	{
		if (!*new_args)
		{
			ft_child_cleaner(sh, new_args, 0);
			ft_exit_status(0, TRUE, TRUE);
		}
		pathname = ft_findpath(sh->global, new_args, sh);
		if (!pathname)
		{
			ft_stderror(FALSE, "%s: command not found", new_args[0]);
			ft_child_cleaner(sh, new_args, 0);
			ft_exit_status(127, TRUE, TRUE);
		}
		if (execve(pathname, new_args, sh->global) == -1)
		{
			ft_stderror(TRUE, "%s: ", new_args[0]);
			ft_child_cleaner(sh, new_args, 0);
			ft_exit_status(1, TRUE, TRUE);
		}
		free(pathname);
	}
	ft_child_cleaner(sh, new_args, 0);
	ft_exit_status(0, FALSE, TRUE);
}
