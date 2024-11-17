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
	static char	special_char;

	if (trim[i] == SQUOTE || trim[i] == DQUOTE)
	{
		i = ft_find_next_quote(trim, i, trim[i]);
		if (i == -1)
			return (printf("%s: open quotes are not supported\n", PROG_NAME), -1); //ft_error_handler();
		*special = false;
	}
	if (ft_strchr(INVALIDCHARS, trim[i]))
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
	if (ft_strchr(METACHARS, trim[i]) || ft_strchr(SPECIALCHARS, trim[i]) || (trim[i] == '.' && (ft_isspace(trim[i + 1]) || trim[i + 1] == '\0')))
	{
		if (*special == true)
		{
			if (special_char == '|')
			{
				if (trim[i] == '|' || ft_strchr(SPECIALCHARS, trim[i]) || (trim[i] == '.' && (ft_isspace(trim[i + 1]) || trim[i + 1] == '\0')))
					return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
			}
			else if (trim[i] != '.')
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
		}
		*special = true;
		special_char = trim[i];
		if (((trim[i] == '<' || trim[i] == '>') && trim[i + 1] == trim[i]) || (trim[i] == '>' && trim[i + 1] == '|'))
			i += 1;
	}
	return (i);
}

/**
 * @brief Checks if a character is an invalid or special starting character for syntax, updating status.
 *
 * Determines if `c` is an invalid or special character for the start of a command or syntax. If `c` is a 
 * comment character (`#`), it sets `*special` to `true` and returns `true` without error. If `c` is `%` or 
 * another special/metacharacter (excluding `<` and `>`), it prints a syntax error message, sets `*special` 
 * to `true`, and returns `true`. If `c` is valid, it sets `*special` to `false` and returns `false`.
 *
 * @param c The character to check.
 * @param special Pointer to a boolean indicating if the character is special.
 * @return `true` if `c` is invalid or special; otherwise, `false`.
 */
static bool	ft_is_invalid_first_char(char *s, bool *special)
{
	if (s[0] == '#') // it indicates a comment
	{
		*special = true;
		return (true); // not an error. doesn't change the last exit_code
	}
	if (s[0] == '%' || ((s[0] == '^' || s[0] == '.') && (ft_isspace(s[1]) || s[1] == '\0')))
	{
		printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[0]); //ft_error_handler();
		*special = true;
		return (true);
	}
	if (ft_strchr(INVALIDCHARS, s[0]) || ft_strchr(SPECIALCHARS, s[0]) || ft_strchr(METACHARS, s[0]))
	{
		if (s[0] != '<' && s[0] != '>')
		{
			printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[0]); //ft_error_handler();
			*special = true;
			return (true);
		}
	}
	*special = false;
	return (false);
}

/**
 * @brief Checks syntax validity of a given input string for proper quote usage, valid metacharacter positioning and env expansion.
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
		return (0); //ft_error_handler(); malloc failed
	i = 0;
	if (ft_is_invalid_first_char(trim, &special))
		return (free(trim), 0);
	while (s[i])
	{
		i = ft_iterate_str(s, i, &special);
		if (i == -1)
			return (0);
		if (s[i] && !ft_isspace(s[i]) && !ft_strchr(METACHARS, s[i]))
			special = false;
		if (s[i])
			i++;
	}
	if (special == true)
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i - 1]), free(trim), 0); //ft_error_handler();
	return (free(trim), 1);
}
