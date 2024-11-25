#include "../../includes/minishell.h"

static int	ft_handle_specialchars(char *trim, int i, bool *is_special, char *c)
{
	if (trim[i] == '&' && trim[i + 1] != '&')
		return (printf(UNEXPECTED_TOKEN, PROG_NAME, trim[i]), -1); //ft_error_handler(); 258
	if (*is_special == true)
	{
		if (*c == '|' || *c == '&') //this is to validate: trailing redirs after both pipes and logic operators are accepted
		{
			if ((*c == '|' && trim[i] == '&') || (*c == '&' && trim[i] == '|'))
				return (printf(UNEXPECTED_TOKEN, PROG_NAME, trim[i]), -1); //ft_error_handler(); 258
			if ((trim[i] == '&' && ft_isspace(trim[i - 1])) || ft_strchr(SPECIALCHARS, trim[i]) || (trim[i] == '.' && (ft_isspace(trim[i + 1]) || trim[i + 1] == '\0')))
				return (printf(UNEXPECTED_TOKEN, PROG_NAME, trim[i]), -1); //ft_error_handler(); 258
			if ((trim[i] == '|' && ft_isspace(trim[i - 1])) || ft_strchr(SPECIALCHARS, trim[i]) || (trim[i] == '.' && (ft_isspace(trim[i + 1]) || trim[i + 1] == '\0')))
				return (printf(UNEXPECTED_TOKEN, PROG_NAME, trim[i]), -1); //ft_error_handler(); 259
		}
		else if (trim[i] != '.') // this means: redirs accept . and .. and ... etc, pipes don't
			return (printf(UNEXPECTED_TOKEN, PROG_NAME, trim[i]), -1); //ft_error_handler(); 258
	}
	*is_special = true;
	*c = trim[i];
	if ((ft_strchr(METACHARS, trim[i]) && trim[i + 1] == trim[i]) || (trim[i] == '>' && trim[i + 1] == '|'))
		i += 1;
	return (i);
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
static int	ft_iterate_str(char *trim, int i, bool *is_special)
{
	//update brief
	static char	special_char;

	if (trim[i] == SQUOTE || trim[i] == DQUOTE)
	{
		i = ft_find_next_quote(trim, i, trim[i]);
		if (i == -1)
			return (printf(OPEN_QUOTE, PROG_NAME), -1); //ft_error_handler(); 2
		*is_special = false;
	}
	if (ft_strchr(INVALIDCHARS, trim[i]))
		return (printf(UNEXPECTED_TOKEN, PROG_NAME, trim[i]), -1); //ft_error_handler(); 258
	if (ft_strchr(METACHARS, trim[i]) || ft_strchr(SPECIALCHARS, trim[i]) || (trim[i] == '.' && (ft_isspace(trim[i + 1]) || trim[i + 1] == '\0')))
	{
		i = ft_handle_specialchars(trim, i, is_special, &special_char);
		if (i == -1)
			return (-1);
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
static bool	ft_is_invalid_first_char(char *s, bool *is_special)
{
	//update brief
	if (s[0] == '#' /* || s[0] == ':' */) // # it indicates a comment. what is :?
	{
		*is_special = true;
		return (true); // not an error. doesn't change the last exit_code
	}
	if (s[0] == '%' || s[0] == '!' || ((s[0] == '^' || s[0] == '.')
			&& (ft_isspace(s[1]) || s[1] == '\0')))
	{
		printf(UNEXPECTED_TOKEN, PROG_NAME, s[0]); //ft_error_handler(); 258
		*is_special = true;
		return (true);
	}
	if (ft_strchr(INVALIDCHARS, s[0]) || ft_strchr(SPECIALCHARS, s[0])
			|| ft_strchr(METACHARS, s[0]) || ft_strchr(PRTHESESCHARS, s[0]))
	{
		if (s[0] != '<' && s[0] != '>' && s[0] != '(')
		{
			printf(UNEXPECTED_TOKEN, PROG_NAME, s[0]); //ft_error_handler(); 258
			*is_special = true;
			return (true);
		}
	}
	*is_special = false;
	return (false);
}

/**
 * @brief Checks syntax validity of a given input string for proper quote usage, valid metacharacter positioning and env expansion.
 *
 * @param input The input string to validate.
 * @return 1 if syntax is valid, otherwise 0.
 */
int	ft_validate_syntax(char *trim)
{
	//update brief
	int		i;
	bool	is_special;

	if (ft_is_invalid_first_char(trim, &is_special) || !ft_validate_parentheses(trim))
		return (0);
	i = 0;
	while (trim[i])
	{
		i = ft_iterate_str(trim, i, &is_special);
		if (i == -1)
			return (0);
		if (trim[i] && !ft_isspace(trim[i]) && !ft_strchr(METACHARS, trim[i]))
		{
			if (trim[i] == '#')
			{
				i++;
				break;
			}
			is_special = false;
		}
		if (trim[i])
			i++;
	}
	if (is_special == true)
		return (printf(UNEXPECTED_TOKEN, PROG_NAME, trim[i - 1]), 0); //ft_error_handler(); 258
	return (1);
}
