#include "../../includes/minishell.h"

/**
 * @brief Validates the balance of parentheses in a string.
 * 
 * This function ensures that every opening parenthesis '(' is properly closed by a 
 * corresponding closing parenthesis ')'. It also skips over characters enclosed in 
 * single or double quotes to avoid misinterpreting symbols within quoted strings.
 * 
 * @param s The input string to validate.
 * @return `true` if all parentheses are balanced; `false` otherwise.
 *         In case of an error (e.g., unbalanced parentheses or open quotes), 
 *         appropriate error messages are printed, and the function terminates with 
 *         an error status.
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
