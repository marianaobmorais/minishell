#include "../../includes/minishell.h"

/**
 * @brief Validates if each argument is a valid environment variable identifier.
 *
 * Checks each string in `argv` to ensure it follows the rules for environment variable names.
 * The first character must be alphabetic or an underscore; subsequent characters may include
 * alphanumeric, underscore, or a plus sign (`+`) only if it is followed by an equals sign (`=`).
 *
 * @param argv Array of argument strings to validate.
 *
 * @return 0 if all identifiers are valid, or an error message and non-zero value if any identifier is invalid.
 */
static int check_key(char **argv)
{
	size_t i;

	while (*argv)
	{
		i = 0;
		if (!ft_isalpha((*argv)[i]) && (*argv)[i] != '_')
			return (ft_stderror(2, "export: '%s' not a valid identifier\n", (*argv)));
		while ((*argv)[i] != '=')
		{
			if (!ft_isalnum((*argv)[i]) && (*argv)[i] != '_')
			{
				if ((*argv)[i] != '+')
					return (ft_stderror(2, "export: '%s' not a valid identifier\n", (*argv)));
				else if ((*argv)[i] == '+' && (*argv)[i + 1] != '=')
					return (ft_stderror(2, "export: '%s' not a valid identifier\n", (*argv)));
			}
			i++;
		}
		argv++;
	}
	return (0);
}

/**
 * @brief Adds a new environment variable to the environment array.
 *
 * Allocates space for a new environment array with one additional slot for `str`,
 * copying existing variables from `my_envp` and appending the new variable.
 *
 * @param str The new environment variable string to add.
 * @param size The current number of environment variables in `my_envp`.
 * @param my_envp Pointer to the environment variable array to be updated.
 */
static void	add_var(char *str, size_t size, char ***my_envp)
{
	int i;
	char **new_envp;

	i = 0;
	new_envp = (char **) malloc((size + 2) * sizeof(char *));
	if (!new_envp)
		exit(1); // tratamento de erro
	i = 0;
	while ((*my_envp)[i])
	{
		new_envp[i] = (*my_envp)[i];
		i++;
	}
	new_envp[i] = ft_strdup(str); // tratatamento de erro
	new_envp[i + 1] = NULL;
	free((*my_envp));
	*my_envp = new_envp;
}

/**
 * @brief Replaces an existing environment variable or adds a new one if not found.
 *
 * Searches for a variable in `my_envp` matching `str` up to the equal sign `=`. 
 * If found, frees the old entry and replaces it with `str`. If not found, 
 * calls `add_var` to add `str` as a new environment variable.
 *
 * @param str The environment variable string to replace or add.
 * @param size The length of the variable name in `str` (up to `=`).
 * @param my_envp Pointer to the array of environment variables to be updated.
 * @return Returns 0 if the variable is replaced, or the new size of `my_envp` if added.
 */
static int replace_var(char *str, size_t size, char ***my_envp)
{
	int i;

	i = 0;
	while ((*my_envp)[i])
	{
		if (ft_strncmp(str, (*my_envp)[i], size) == 0
			&& (*my_envp)[i][size] == '=')
		{
			free((*my_envp)[i]);
			(*my_envp)[i] = ft_strdup(str);
			return (0);
		}
		i++;
	}
	add_var(str, i, my_envp);
	return (i);
}

/**
 * @brief Concatenates a value to an existing environment variable or adds it as a new variable.
 *
 * Searches for an environment variable in `my_envp` that matches the prefix of `str` (before `+=`).
 * If found, concatenates the new value from `str` (after `+=`) to the existing variable value.
 * If not found, calls `add_var` to add `str` as a new environment variable.
 *
 * @param str The environment variable string with the format "VAR+=VALUE".
 * @param my_envp Pointer to the array of environment variables to be updated.
 * @return Returns 0 if the variable is concatenated, or the new size of `my_envp` if added.
 */
static int concatenate_var(char *str, char ***my_envp)
{
	int		i;
	int		size;
	char	*temp_str;
	char	*value;

	i = 0;
	while ((*my_envp)[i])
	{
		value = ft_strchr(str, '+') + 2;
		size = (ft_strlen(str) - ft_strlen(value)) - 2;
		if (ft_strncmp(str, (*my_envp)[i], size) == 0
			&& (*my_envp)[i][size] == '=')
		{
			temp_str = ft_strjoin((*my_envp)[i], value);
			free((*my_envp)[i]);
			(*my_envp)[i] = temp_str;
			return (0);
		}
		i++;
	}
	add_var(str, i, my_envp);
	return (i);
}

/**
 * @brief Handles the exportation of environment variables.
 *
 * This function manages the exportation of environment variables. If no arguments are passed,
 * it prints the current environment variables. If arguments are passed, it checks for valid keys,
 * and either concatenates or replaces the environment variable based on the input.
 *
 * @param argc The number of arguments passed to the function.
 * @param argv An array of strings representing the arguments.
 * @param my_envp A pointer to the array of environment variables to be updated.
 * @return Returns 0 on success, or the exit status code for various error conditions.
 */
int	ft_export(int argc, char **argv, char ***my_envp)
{
	size_t s_key;

	if (argc == 1)
	{
		ft_print_export(*my_envp);
		return (ft_exit_status(0, TRUE, FALSE));
	}
	argv++;
	if (check_key(argv) != 0)
		return (ft_exit_status(2, TRUE, FALSE));
	while (*argv)
	{
		if (!ft_strchr(*argv, '='))
			return (ft_exit_status(0, TRUE, FALSE));
		// nao fazer nada e guardar junto com as variaveis que foram exportadas
		s_key = (ft_strlen(*argv) - ft_strlen(ft_strchr(*argv, '=')));
		if ((*argv)[s_key - 1] == '+')
			concatenate_var(*argv, my_envp);
		else
			replace_var(*argv, s_key, my_envp);
		argv++;
	}
	return (ft_exit_status(0, TRUE, FALSE));
}
