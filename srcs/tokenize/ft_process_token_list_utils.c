#include "../../includes/minishell.h"

/**
 * @brief Expands an environment variable found in a string.
 * 
 * This function extracts an environment variable name from the input string and searches 
 * for its value in the environment variable array. If found, it returns the corresponding 
 * value; otherwise, it returns an empty string. It updates the index to skip past the 
 * environment variable name.
 * 
 * @param s Pointer to the current position in the input string.
 * @param i Pointer to the index, which is updated to skip past the variable name.
 * @param my_envp Array of environment variables.
 * @return The expanded environment variable value or an empty string.
 */
char	*ft_expand_env(char *s, int *i, char **my_envp)
{
	char	*expansion;
	char	*env;
	char	*env_equal;
	int		len;
	int		j;

	len = 0;
	while (ft_isalpha(s[len]) || ft_isdigit(s[len]) || s[len] == '_')
		len++;
	env = ft_substr(s, 0, len);
	env_equal = ft_strjoin(env, "=");
	j = 0;
	while (my_envp[j])
	{
		if (ft_strncmp(env_equal, my_envp[j], (len + 1)) == 0)
		{
			expansion = ft_strdup(my_envp[j] + (len + 1));
			(*i) += len; // update index past env name
			return (free(env_equal), expansion);
		}
		j++;
	}
	(*i) += len; // update index past env name
	return (free(env_equal), ft_strdup("\0")); // this is not an error
}

/**
 * @brief Retrieves the exit code as a string.
 * 
 * This function retrieves the last process's exit code. It updates the index 
 * and returns the exit_code in string format.
 * 
 * @param i Pointer to the index, incremented to skip past the special character.
 * @return Exit code value in string format.
 */
char	*ft_get_exit_code(int *i)
{
	(*i)++;
	return (ft_strdup("EXIT_CODE")); // to do
}

/**
 * @brief Handles environment variable expansion in a string.
 * 
 * This function manages the expansion of `$`-prefixed variables in the string. If the `$` 
 * is followed by a `?`, it expands to the exit code. Otherwise, it expands to the 
 * environment variable value if available, updating `new_value` with the expanded result.
 * 
 * @param new_value Pointer to the string being built with expansions.
 * @param value Original input string.
 * @param i Pointer to the index, updated to skip past the expansion.
 * @param my_envp Array of environment variables.
 */
void	ft_handle_expansion(char **new_value, char *value, int *i, char **my_envp)
{
	char	*expansion;
	char	*tmp;

	(*i)++; // skip $
	if (value[(*i)] == '?')
		expansion = ft_get_exit_code(i);
	else if (ft_isdigit(value[(*i)]))
	{
		expansion = ft_strdup("\0"); // this is not an error
		(*i)++;
	}
	else
		expansion = ft_expand_env(&value[(*i)], i, my_envp);
	tmp = ft_strjoin(*new_value, expansion);
	free(*new_value);
	free(expansion);
	*new_value = tmp;
}

/**
 * @brief Processes single-quoted text in a string.
 * 
 * This function appends characters within single quotes to `new_value`, preserving the 
 * quotes, and updates the index to skip past the quoted section.
 * 
 * @param new_value Pointer to the string being built.
 * @param value Original input string.
 * @param i Pointer to the index, updated to skip past the quoted section.
 */
void	ft_handle_squotes(char **new_value, char *value, int *i)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[(*i)] && value[(*i)] != SQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
}

/**
 * @brief Processes double-quoted text with potential variable expansion.
 * 
 * This function appends characters within double quotes to `new_value`, performing 
 * environment variable expansion for `$` symbols where applicable. It updates the index 
 * to skip past the quoted section.
 * 
 * @param new_value Pointer to the string being built with expansions.
 * @param value Original input string.
 * @param i Pointer to the index, updated to skip past the quoted section.
 * @param my_envp Array of environment variables.
 */
void	ft_handle_dquotes(char **new_value, char *value, int *i, char **my_envp)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[(*i)] && value[(*i)] != DQUOTE)
	{
		if (value[(*i)] == '$' && ((ft_isalnum(value[(*i) + 1]) || value[(*i) + 1] == '?' || value[(*i) + 1] == '_')))
			ft_handle_expansion(new_value, value, i, my_envp);
		else
			*new_value = ft_charjoin(*new_value, value[(*i)++]);
	}
	if (value[(*i)] == DQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
}

