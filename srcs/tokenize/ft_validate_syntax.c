#include "../../includes/minishell.h"

/**
 * @brief Handles syntax errors by printing an error message and updating the exit status.
 * 
 * This function outputs a syntax error message, optionally including the character 
 * that caused the error. It also sets the exit status to a predefined error value.
 * 
 * @param message The error message to be displayed.
 * @param c The optional character causing the syntax error. If `0`, no character is displayed.
 */
void	ft_error_syntax(char *message, char c)
{
	if (c != 0)
		ft_stderror(FALSE, message, c);
	else
		ft_stderror(FALSE, message);
	ft_exit_status(2, TRUE, FALSE);
}

/**
 * @brief Handles and validates special characters in a trimmed input string.
 * 
 * This function checks for syntax errors related to special characters (e.g.,
 * `&`, `|`, redirections) in the input. It identifies unexpected tokens,
 * validates character sequences, and updates the state of the `is_special`
 * flag and the current special character tracker. If an error is encountered,
 * an appropriate error message is printed, and the function returns `-1`.
 * Otherwise, it returns the updated index after handling multi-character
 * tokens.
 * 
 * @param trim The trimmed input string being validated.
 * @param i The current index in the input string.
 * @param special Pointer to a flag indicating if the previous character
 *                was special.
 * @param c Pointer to the current special character being tracked.
 * @return The updated index after handling special characters, or `-1`
 *         on error.
 */
static int	ft_handle_specialchars(char *s, int i, bool *special, char *c)
{
	if (s[i] == '&' && s[i + 1] != '&')
		return (ft_error_syntax(UNEXPECTED_TOKEN, s[i]), -1);
	if (*special == true)
	{
		if (*c == '|' || *c == '&')
		{
			if ((*c == '|' && s[i] == '&') || (*c == '&' && s[i] == '|'))
				return (ft_error_syntax(UNEXPECTED_TOKEN, s[i]), -1);
			if (!ft_validate_logic_operator(s, i))
				return (ft_error_syntax(UNEXPECTED_TOKEN, s[i]), -1);
		}
		else if (s[i] != '.')
			return (ft_error_syntax(UNEXPECTED_TOKEN, s[i]), -1);
	}
	*special = true;
	*c = s[i];
	if ((ft_strchr(METACHARS, s[i]) && s[i + 1] == s[i])
		|| (s[i] == '>' && s[i + 1] == '|'))
		i += 1;
	return (i);
}

/**
 * @brief Processes a string character by character to validate and identify
 *        special syntax.
 * 
 * This function iterates through a string, checking for special characters,
 * quotes, and invalid tokens. It handles special syntax like quotes, meta-
 * characters, and unexpected tokens. If a syntax issue is encountered, an
 * error is reported, and the function returns `-1`. It also updates the
 * special` flag to indicate whether the current character is part of a special
 * sequence.
 * 
 * @param s The input string to process.
 * @param i The current index in the string.
 * @param special Pointer to a boolean flag indicating if the current character
 *                is special.
 * @return The updated index after processing the current character, or `-1` if
 *         an error occurs.
 */
static int	ft_iterate_str(char *s, int i, bool *special)
{
	static char	special_char;

	if (s[i] == SQUOTE || s[i] == DQUOTE)
	{
		i = ft_find_next_quote(s, i, s[i]);
		if (i == -1)
			return (ft_error_syntax(OPEN_QUOTE, 0), -1);
		*special = false;
	}
	if (ft_strchr(INVALIDCHARS, s[i]))
		return (ft_error_syntax(UNEXPECTED_TOKEN, s[i]), -1);
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
 * @brief Validates the first character of a string for syntax correctness.
 * 
 * This function checks if the first character of the input string is valid
 * according to predefined rules. It identifies special cases like comments
 * (`#`), invalid characters, and unsupported tokens. If the first character
 * is invalid, an error is reported, the exit status is updated, and the
 * function returns `true`. Otherwise, it updates the `special` flag and
 * returns `false`.
 * 
 * @param s The input string to validate.
 * @param special Pointer to a boolean flag indicating if the first character
 *                is special.
 * @return `true` if the first character is invalid; `false` otherwise.
 */
static bool	ft_invalid_first_chr(char *s, bool *special)
{
	if (s[0] == '#' || s[0] == ':')
		return (true);
	if (s[0] == '%' || s[0] == '!' || ((s[0] == '^' || s[0] == '.')
			&& (ft_isspace(s[1]) || s[1] == '\0')))
	{
		ft_error_syntax(UNEXPECTED_TOKEN, s[0]);
		return (true);
	}
	if (ft_strchr(INVALIDCHARS, s[0]) || ft_strchr(SPECIALCHARS, s[0])
		|| ft_strchr(METACHARS, s[0]) || ft_strchr(PRTHESESCHARS, s[0]))
	{
		if (s[0] != '<' && s[0] != '>' && s[0] != '(')
		{
			ft_error_syntax(UNEXPECTED_TOKEN, s[0]);
			return (true);
		}
	}
	*special = false;
	return (false);
}

/**
 * @brief Validates the syntax of a trimmed shell input string.
 * 
 * This function checks the input string for syntax errors, including invalid
 * characters, unbalanced parentheses and quotes, and improper placement of
 * special characters (e.g., pipes, redirections). It iterates through the
 * string, updating the state of special characters and comments, and handles
 * errors with appropriate messages and exit codes. The function ensures that
 * the input adheres to shell syntax rules before proceeding to parsing or
 * execution.
 * 
 * @param trim The trimmed input string to validate.
 * @return `true` if the syntax is valid, otherwise `false`.
 */
bool	ft_validate_syntax(char *trim)
{
	int		i;
	bool	special;

	if (ft_invalid_first_chr(trim, &special) || !ft_count_parentheses(trim)
		|| !ft_validate_parentheses(trim))
		return (false);
	i = 0;
	while (trim[i])
	{
		i = ft_iterate_str(trim, i, &special);
		if (i == -1)
			return (false);
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
		return (ft_error_syntax(UNEXPECTED_TOKEN, trim[i - 1]), 0);
	return (true);
}
