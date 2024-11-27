#include "../../includes/minishell.h"

/**
 * @brief Handles and validates special characters in a trimmed input string.
 * 
 * This function checks for syntax errors related to special characters (e.g., `&`, `|`, redirections)
 * in the input. It identifies unexpected tokens, validates character sequences, and updates the
 * state of the `is_special` flag and the current special character tracker. If an error is encountered,
 * an appropriate error message is printed, and the function returns `-1`. Otherwise, it returns the
 * updated index after handling multi-character tokens.
 * 
 * @param trim The trimmed input string being validated.
 * @param i The current index in the input string.
 * @param special Pointer to a flag indicating if the previous character was special.
 * @param c Pointer to the current special character being tracked.
 * @return The updated index after handling special characters, or `-1` on error.
 */
static int	ft_handle_specialchars(char *s, int i, bool *special, char *c)
{
	if (s[i] == '&' && s[i + 1] != '&')
		return (ft_stderror(FALSE, UNEXPECTED_TOKEN, s[i]), ft_exit_status(2, TRUE, FALSE), -1);
	if (*special == true)
	{
		if (*c == '|' || *c == '&')
		{
			if ((*c == '|' && s[i] == '&') || (*c == '&' && s[i] == '|'))
				return (ft_stderror(FALSE, UNEXPECTED_TOKEN, s[i]), ft_exit_status(2, TRUE, FALSE), -1);
			if (((s[i] == '&' || s[i] == '|') && ft_isspace(s[i - 1]))
					|| ft_strchr(SPECIALCHARS, s[i])
					|| (s[i] == '.' && (ft_isspace(s[i + 1])
					|| s[i + 1] == '\0')))
				return (ft_stderror(FALSE, UNEXPECTED_TOKEN, s[i]), ft_exit_status(2, TRUE, FALSE), -1); //ft_error_handler(); 2
		}
		else if (s[i] != '.')
			return (ft_stderror(FALSE, UNEXPECTED_TOKEN, s[i]), ft_exit_status(2, TRUE, FALSE), -1); //ft_error_handler(); 2
	}
	*special = true;
	*c = s[i];
	if ((ft_strchr(METACHARS, s[i]) && s[i + 1] == s[i])
			|| (s[i] == '>' && s[i + 1] == '|'))
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
static int	ft_iterate_str(char *s, int i, bool *special)
{
	//update brief
	static char	special_char;

	if (s[i] == SQUOTE || s[i] == DQUOTE)
	{
		i = ft_find_next_quote(s, i, s[i]);
		if (i == -1)
			return (ft_stderror(FALSE, OPEN_QUOTE), ft_exit_status(2, TRUE, FALSE), -1); //ft_error_handler(); 2
		*special = false;
	}
	if (ft_strchr(INVALIDCHARS, s[i]))
		return (ft_stderror(FALSE, UNEXPECTED_TOKEN, s[i]), ft_exit_status(2, TRUE, FALSE), -1); //ft_error_handler(); 2
	if (ft_strchr(METACHARS, s[i]) || ft_strchr(SPECIALCHARS, s[i])
			|| (s[i] == '.' && (ft_isspace(s[i + 1])
			|| s[i + 1] == '\0')))
	{
		i = ft_handle_specialchars(s, i, special, &special_char);
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
static bool	ft_invalid_first_chr(char *s, bool *special)
{
	//update brief
	if (s[0] == '#' /* || s[0] == ':' */) // # it indicates a comment. what is :?
		return (true); // not an error. doesn't change the last exit_code
	if (s[0] == '%' || s[0] == '!' || ((s[0] == '^' || s[0] == '.')
			&& (ft_isspace(s[1]) || s[1] == '\0')))
	{
		ft_stderror(FALSE, UNEXPECTED_TOKEN, s[0]);
		ft_exit_status(2, TRUE, FALSE);
		return (true);
	}
	if (ft_strchr(INVALIDCHARS, s[0]) || ft_strchr(SPECIALCHARS, s[0])
			|| ft_strchr(METACHARS, s[0]) || ft_strchr(PRTHESESCHARS, s[0]))
	{
		if (s[0] != '<' && s[0] != '>' && s[0] != '(')
		{
			ft_stderror(FALSE, UNEXPECTED_TOKEN, s[0]);
			ft_exit_status(2, TRUE, FALSE);
			return (true);
		}
	}
	*special = false;
	return (false);
}

bool	ft_is_comment(char c, int *i)
{
	if (c == '#')
	{
		(*i)++;
		return (true);
	}
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
	int		i; //update brief
	bool	special;

	if (ft_invalid_first_chr(trim, &special) || !ft_validate_parentheses(trim))
		return (0);
	i = 0;
	while (trim[i])
	{
		i = ft_iterate_str(trim, i, &special);
		if (i == -1)
			return (0);
		if (trim[i] && !ft_isspace(trim[i]) && !ft_strchr(METACHARS, trim[i]))
		{
			if (ft_is_comment(trim[i], &i))
				break ;
			special = false;
		}
		if (trim[i])
			i++;
	}
	if (special == true)
		return (ft_stderror(FALSE, "syntax error near unexpected token `%c'", trim[i - 1]),
				ft_exit_status(2, TRUE, FALSE), 0);
	return (1);
}
