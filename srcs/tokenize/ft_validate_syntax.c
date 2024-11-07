#include "../../includes/minishell.h"

/**
 * @brief Checks if a character is a whitespace.
 * 
 * This function returns 1 if the character is a space (ASCII 32) or a whitespace character 
 * within the range of ASCII 9 to 13, otherwise returns 0.
 * 
 * @param c The character to check.
 * @return 1 if whitespace, otherwise 0.
 */
int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

/**
 * @brief Iterates over a string to check for unmatched quotes or invalid syntax characters.
 * 
 * This function navigates through the `trim` string, checking if characters are quotes, 
 * special characters, or metacharacters. It detects and handles cases where quotes are 
 * unclosed or special characters appear unexpectedly.
 * 
 * @param trim The trimmed input string.
 * @param i Current index in the string.
 * @param special Pointer to a boolean that tracks consecutive special characters.
 * @return Updated index if successful, -1 if syntax error.
 */
static int	ft_iterate_str(char *trim, int i, bool *special)
{
	if (trim[i] == 39 || trim[i] == 34)
	{
		i = ft_find_next_quote(trim, i, trim[i]);
		if (i == -1)
			return (printf("%s: open quotes are not supported\n", PROG_NAME), -1); //ft_error_handler();
		*special = false;
	}
	if (ft_strchr(SPECIALCHARS, trim[i]))
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
	if (ft_strchr(METACHARS, trim[i]))
	{
		if (*special == true)
			return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
		*special = true;
		if ((trim[i] == '<' || trim[i] == '>') && trim[i + 1] == trim[i])
			i += 1;
	}
	return (i);
}
/**
 * @brief Validates if the first character in a string is an unexpected metacharacter or special character.
 *
 * @param c The character to check.
 * @return true if the first character is invalid, otherwise false.
 */
static bool	ft_is_invalid_first_char(char c)
{
	if (ft_strchr(SPECIALCHARS, c) || ft_strchr(METACHARS, c))
	{
		if (c != '<' && c != '>')
		{
			printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, c); //ft_error_handler();
			return (true);
		}
	}
	return (false);
}
/**
 * @brief Checks syntax validity of a given input string for proper quote usage and valid metacharacter positioning.
 *
 * @param input The input string to validate.
 * @return 1 if syntax is valid, otherwise 0.
 */
int	ft_validate_syntax(char *s)
{
	char	*trim;
	int		i;
	bool	special;

	trim = ft_strtrim(s, ISSPACE);
	if (!trim)
		return (0); //ft_error_handler();
	i = 0;
	special = ft_is_invalid_first_char(trim[i]);
	if (special)
		return (free(trim), 0);
	while (trim[i])
	{
		i = ft_iterate_str(trim, i, &special);
		if (i == -1)
			return (free(trim), 0);
		if (trim[i] && !ft_isspace(trim[i]) && !ft_strchr(METACHARS, trim[i]))
			special = false;
		if (trim[i])
			i++;
	}
	if (special == true)
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i - 1]), free(trim), 0); //ft_error_handler();
	return (free(trim), 1);
}
