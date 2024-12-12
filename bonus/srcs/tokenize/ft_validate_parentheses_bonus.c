#include "../../includes/minishell_bonus.h"

/**
 * @brief Validates the placement and context of parentheses during parsing.
 * 
 * This function checks if the given parenthesis character is used correctly:
 * - Ensures '(' is not preceded by invalid characters like '<', '>', or
 *   alphanumeric characters.
 * - Ensures ')' does not appear after a '|' or in an invalid left context.
 * 
 * It updates the tracking flags for left (`(`) and right (`)`) parentheses and
 * raises syntax errors for unexpected tokens.
 * 
 * @param c The current character being checked.
 * @param last The previous significant character in the input string.
 * @param left Pointer to the flag tracking the presence of unmatched left
 *        parentheses.
 * @param right Pointer to the flag tracking the presence of unmatched right
 *        parentheses.
 * @return `true` if the character is valid in its context; `false` otherwise,
 *         with an error message printed and the exit status updated.
 */
static bool	ft_handle_paretheses(char c, char *last, bool *left, bool *right)
{
	if (c == '(')
	{
		if (*last == '<' || *last == '>' || ft_isalnum(*last))
		{
			ft_stderror(FALSE, UNEXPECTED_TOKEN, c);
			ft_exit_status(2, TRUE, FALSE);
			return (false);
		}
		*left = true;
	}
	else if (c == ')')
	{
		if (*left || *last == '|')
		{
			ft_stderror(FALSE, UNEXPECTED_TOKEN, c);
			ft_exit_status(2, TRUE, FALSE);
			return (false);
		}
		*right = true;
	}
	return (true);
}

/**
 * @brief Ensures valid syntax following a left parenthesis.
 * 
 * This function checks the context after encountering a left parenthesis '('.
 * It ensures that invalid tokens like '|' or dangling '.' do not immediately 
 * follow a '(' without appropriate context.
 * 
 * @param s The input string being parsed.
 * @param i The current index in the string.
 * @param left Pointer to the flag tracking the presence of an unmatched left
 *        parenthesis.
 * @return `true` if the context after the left parenthesis is valid; `false`
 *         otherwise, and an error message printed and the exit status updated.
 */
static bool	ft_validate_left_context(char *s, int i, bool *left)
{
	if (*left && (s[i] == '|'
			|| (s[i] == '.' && (ft_isspace(s[i + 1]) || s[i + 1] == ')'))))
	{
		ft_stderror(FALSE, UNEXPECTED_TOKEN, s[i]);
		ft_exit_status(2, TRUE, FALSE);
		return (false);
	}
	return (true);
}

/**
 * @brief Handles the validation of characters encountered outside parentheses.
 * 
 * This function processes characters that are not parentheses, ensuring:
 * - Quoted strings are handled by skipping to the closing quote.
 * - Right parentheses ')' are followed by valid operators like '|', '&', '>',
 *   or '<'.
 * - No unexpected tokens appear after a right parenthesis.
 * 
 * It also resets the left and right parentheses flags when valid characters are
 * encountered.
 * 
 * @param s The input string being parsed.
 * @param i Pointer to the current index in the string.
 * @param right Pointer to the flag tracking the presence of an unmatched right
 *        parenthesis.
 * @param left Pointer to the flag tracking the presence of an unmatched left
 *        parenthesis.
 * @return `true` if the character is valid in its context; `false` otherwise,
 *         with an error message printed and the exit status updated.
 */
static bool	ft_handle_chars(char *s, int *i, bool *right, bool *left)
{
	if (s[*i] == DQUOTE || s[*i] == SQUOTE)
		*i = ft_find_next_quote(s, *i, s[*i]);
	else if (*right && s[*i] != '|' && s[*i] != '&'
		&& s[*i] != '>' && s[*i] != '<')
	{
		ft_stderror(FALSE, UNEXPECTED_TOKEN, ')');
		ft_exit_status(2, TRUE, FALSE);
		return (false);
	}
	*left = false;
	*right = false;
	return (true);
}

/**
 * @brief Validates the balance of parentheses in a string.
 * 
 * This function ensures that every opening parenthesis '(' is properly closed
 * by a corresponding closing parenthesis ')'. It also skips over characters
 * enclosed in single or double quotes to avoid misinterpreting symbols within
 * quoted strings.
 * 
 * @param s The input string to validate.
 * @return `true` if all parentheses are balanced; `false` otherwise.
 *         In case of an error (e.g., unbalanced parentheses or open quotes), 
 *         appropriate error messages are printed, and the function terminates
 *         with an error status.
 */
bool	ft_count_parentheses(char *s)
{
	int	i;
	int	count;

	count = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] == '(')
			count++;
		else if (s[i] == ')')
		{
			if (count == 0)
				return (ft_error_syntax(UNEXPECTED_TOKEN, s[i]), false);
			count--;
		}
		else if (s[i] == DQUOTE || s[i] == SQUOTE)
		{
			i = ft_find_next_quote(s, i, s[i]);
			if (i == -1)
				return (ft_error_syntax(OPEN_QUOTE, 0), false);
		}
	}
	if (count == 0)
		return (true);
	return (ft_stderror(FALSE, OPEN_PRTHESES), false);
}

/**
 * @brief Checks for valid placement and usage of parentheses in a string.
 * 
 * This function analyzes the context of each parenthesis in the string,
 * ensuring:
 * - Parentheses are properly opened and closed.
 * - The context of opening and closing parentheses is logically valid.
 * - Symbols immediately adjacent to parentheses conform to the expected
 *   syntax.
 * 
 * Additionally, it tracks the left and right parentheses' state, ensuring
 * proper nesting and no dangling parentheses at the end of the string.
 * 
 * @param s The input string to validate.
 * @return `true` if the parentheses are valid; `false` otherwise.
 *         In case of errors (e.g., unexpected tokens or unbalanced
 *         parentheses), the function prints error messages and updates the
 *         exit status.
 */
bool	ft_validate_parentheses(char *s)
{
	int		i;
	bool	left;
	bool	right;
	char	last;

	left = false;
	right = false;
	last = ' ';
	i = -1;
	while (s[++i])
	{
		if (!ft_handle_paretheses(s[i], &last, &left, &right))
			return (false);
		else if (!ft_validate_left_context(s, i, &left))
			return (false);
		else if (s[i] != '(' && s[i] != ')' && !ft_isspace(s[i]))
		{
			if (!ft_handle_chars(s, &i, &right, &left))
				return (false);
			last = s[i];
		}
	}
	if (left)
		return (ft_error_syntax(UNEXPECTED_TOKEN, '('), false);
	return (true);
}
