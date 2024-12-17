#include "../../includes/minishell.h"

/**
 * @brief Adds a new string to a dynamically allocated string vector.
 * 
 * Creates a new vector by appending a given string to an existing vector of 
 * strings. Frees the original vector and duplicates its contents into the
 * new one.
 * 
 * @param old_vector Pointer to the original string vector, or NULL if 
 *                   creating a new vector.
 * @param new_str String to add to the vector.
 * @return A pointer to the new vector with the appended string, or NULL if 
 *         allocation fails.
 */
char	**ft_add_to_vector(char **old_vector, char *new_str)
{
	char	**new_vector;
	int		i;
	int		size;

	i = 0;
	size = ft_argslen(old_vector);
	new_vector = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_vector)
		return (ft_error_malloc("new_vector"), NULL);
	*new_vector = NULL;
	i = 0;
	if (!old_vector)
		new_vector[i++] = ft_strdup(new_str);
	{
		while (old_vector[i])
		{
			new_vector[i] = ft_strdup(old_vector[i]);
			i++;
		}
		new_vector[i++] = ft_strdup(new_str);
		ft_free_vector(old_vector);
	}
	new_vector[i] = NULL;
	return (new_vector);
}

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

	if (stat(pathname, &file) == -1)
	{
		ft_stderror(TRUE, "%s: ", args[0]);
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
		ft_stderror(TRUE, "%s: ", args[0]);
		ft_child_cleaner(sh, args, 0);
		ft_exit_status(126, TRUE, TRUE);
		return (-1);
	}
	return (0);
}

/**
 * @brief Validates if the given pathname points to an executable file.
 *
 * This function checks if the provided pathname points to a valid file that
 * is not a directory and has read and execute permissions. If the file is
 * invalid, it prints an error message, cleans up resources, and exits with
 * a status code of 126.
 *
 * @param pathname The path of the file to validate.
 * @param args An array of arguments, where args[0] is used for error messages.
 * @param sh A pointer to the shell structure for cleanup purposes.
 * @return 0 if the file is valid, -1 otherwise.
 */
int	isvalid_file(char *pathname, char **args, t_shell *sh)
{
	struct stat	file;

	if (stat(pathname, &file) == -1)
	{
		ft_stderror(TRUE, "%s: ", args[0]);
		ft_child_cleaner(sh, args, 0);
		ft_exit_status(126, TRUE, TRUE);
		return (-1);
	}
	if (S_ISDIR(file.st_mode) != 0)
		return (-1);
	if (access(pathname, R_OK) == -1)
		return (-1);
	if (access(pathname, X_OK) == -1)
		return (-1);
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
char	*merge(char *s1, char *s2)
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
int	ft_free_paths(char **paths, int i)
{
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (0);
}
