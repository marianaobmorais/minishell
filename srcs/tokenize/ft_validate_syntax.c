#include "../../includes/minishell.h"

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
	//update brief
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
		if (trim[i] == '&' && trim[i + 1] != '&')
			return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1);
		if (*special == true)
		{
			if (special_char == '|' || special_char == '&') //this is to validate: trailing redirs after pipes (it is accepted)
			{
				if ((trim[i] == '&' && ft_isspace(trim[i - 1])) || ft_strchr(SPECIALCHARS, trim[i]) || (trim[i] == '.' && (ft_isspace(trim[i + 1]) || trim[i + 1] == '\0')))
					return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
				if ((trim[i] == '|' && ft_isspace(trim[i - 1])) || ft_strchr(SPECIALCHARS, trim[i]) || (trim[i] == '.' && (ft_isspace(trim[i + 1]) || trim[i + 1] == '\0')))
					return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
			}
			else if (trim[i] != '.') // this means: redirs accept . and .. and ... etc (BUT NEED TO STUDY THIS), pipes don't
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
		}
		*special = true;
		special_char = trim[i];
		if ((ft_strchr(METACHARS, trim[i]) && trim[i + 1] == trim[i]) || (trim[i] == '>' && trim[i + 1] == '|'))
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
	//update brief
	if (s[0] == '#' /* || s[0] == ':' */) // # it indicates a comment. what is :?
	{
		*special = true;
		return (true); // not an error. doesn't change the last exit_code
	}
	if (s[0] == '%' || s[0] == '!' || ((s[0] == '^' || s[0] == '.')
			&& (ft_isspace(s[1]) || s[1] == '\0')))
	{
		printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[0]); //ft_error_handler();
		*special = true;
		return (true);
	}
	if (ft_strchr(INVALIDCHARS, s[0]) || ft_strchr(SPECIALCHARS, s[0])
			|| ft_strchr(METACHARS, s[0]) || ft_strchr(PRTHESESCHARS, s[0]))
	{
		if (s[0] != '<' && s[0] != '>' && s[0] != '(')
		{
			printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[0]); //ft_error_handler();
			*special = true;
			return (true);
		}
	}
	*special = false;
	return (false);
}

bool ft_count_parentheses(char *s)
{
	int	i; //write brief
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == '(')
			count++;
		else if (s[i] == ')')
		{
			if (count == 0)
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]), false); //ft_error_handler();
			count--;
		}
		else if (s[i] == DQUOTE || s[i] == SQUOTE)
			i = ft_find_next_quote(s, i, s[i]);
		i++;
	}
	if (count == 0)
		return (true);
	return (printf("%s: open parentheses are not supported\n", PROG_NAME), false); //ft_error_handler();
 
}

bool ft_validate_parenthesis(char *s)
{
	//write brief
	int		i;
	bool	left;
	bool	right;
	char	c;

	if (!ft_count_parentheses(s))
		return (false);
	i = 0;
	left = false;
	right = false;
	while (s[i])
	{
		if (s[i] == '(')
		{
			if (i > 0 && (c == '<' || c == '>' || ft_isalnum(c)))
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]), false); //ft_error_handler();
			left = true;
		}
		else if (s[i] == ')')
		{
			if (left || c == '|')
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]), false); //ft_error_handler();
			right = true;
		}
		else if (left && s[i] == '|')
			return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]), false); //ft_error_handler();
		else if (s[i] == DQUOTE || s[i] == SQUOTE)
			i = ft_find_next_quote(s, i, s[i]);
		else if (!ft_isspace(s[i]))
		{
			if (right && s[i] != '|' && s[i] != '&')
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, ')'), false); //ft_error_handler();
			c = s[i];
			left = false;
			right = false;
		}
		i++;
	}
	if (left)
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, '('), false); //ft_error_handler();
	return (true);
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
	bool	special;

	if (ft_is_invalid_first_char(trim, &special) || !ft_validate_parenthesis(trim))
		return (0);
	i = 0;
	while (trim[i])
	{
		i = ft_iterate_str(trim, i, &special);
		if (i == -1)
			return (0);
		if (trim[i] && !ft_isspace(trim[i]) && !ft_strchr(METACHARS, trim[i]))
		{
			if (trim[i] == '#')
			{
				i++;
				break;
			}
			special = false;
		}
		if (trim[i])
			i++;
	}
	if (special == true)
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i - 1]), 0); //ft_error_handler();
	return (1);
}
