#include "../../includes/minishell.h"

/**
 * @brief Handles quoted segments and environment variable expansions.
 *
 * Processes quoted segments within a string, handling both double and single
 * quotes. Expands environment variables within the quoted segment as needed.
 *
 * @param new_value Pointer to the resulting string with quotes and expansions.
 * @param value The original string containing quotes and variables.
 * @param i Pointer to the current index in the original string.
 * @param envp Array of environment variables for expansion.
 */
void	ft_handle_quotes_hd(char **new_value, char *value, int *i, char **envp)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[(*i)] && value[(*i)] != DQUOTE && value[(*i)] != SQUOTE)
	{
		if (value[(*i)] == '$'
			&& ((ft_isalnum(value[(*i) + 1])
					|| value[(*i) + 1] == '?' || value[(*i) + 1] == '_')))
			ft_handle_expansion(new_value, value, i, envp);
		else
			*new_value = ft_charjoin(*new_value, value[(*i)++]);
	}
	if (value[(*i)] == DQUOTE || value[(*i)] == SQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
}

/**
 * @brief Expands environment variables and handles quoted segments in input.
 *
 * Processes the input string, expanding environment variables and handling
 * quoted segments. It supports both single and double quotes and handles
 * special characters like '$'.
 *
 * @param input The original input string to be expanded.
 * @param envp Array of environment variables for expansion.
 *
 * @return A newly allocated str with expanded variables and processed quotes.
 */
char	*ft_expand_input(char *input, char **envp)
{
	char	*new_input;
	int		i;

	new_input = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == DQUOTE || input[i] == SQUOTE)
			ft_handle_quotes_hd(&new_input, input, &i, envp);
		else if (input[i] == '$' && (ft_isalnum(input[i + 1])
				|| input[i + 1] == '?' || input[i + 1] == '_'))
			ft_handle_expansion(&new_input, input, &i, envp);
		else if (input[i] && input[i] != DQUOTE && input[i] != SQUOTE)
			new_input = ft_charjoin(new_input, input[i++]);
	}
	free(input);
	return (new_input);
}

/**
 * @brief Manages and returns a static line counter.
 *
 * Initializes the line counter if it is zero and increments the counter
 * when mode is set to 1. Returns the current value of the line counter.
 *
 * @param mode Determines if the line counter should be incremented 
 *             (1 to increment).
 *
 * @return The current value of the line counter.
 */
int	count_line(int mode)
{
	static int	line;

	if (line == 0)
		line = 1;
	if (mode == 1)
		line++;
	return (line);
}

/**
 * @brief Creates a unique pathname in the /tmp directory.
 *
 * Generates a unique pathname by appending an incrementing integer to the
 * base path "/tmp/.heredoc_". Frees temporary resources and increments the
 * static counter for the next call.
 *
 * @return A newly allocated string containing the unique pathname.
 */
char	*ft_create_pathname(void)
{
	char		*base;
	char		*pathname;
	char		*temp;
	static int	i;

	base = "/tmp/.heredoc_";
	temp = ft_itoa(i);
	pathname = ft_strjoin(base, temp);
	free(temp);
	unlink(pathname);
	i++;
	return (pathname);
}
