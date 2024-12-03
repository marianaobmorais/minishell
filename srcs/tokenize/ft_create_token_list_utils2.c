#include "../../includes/minishell.h"

/**
 * @brief Determines if the last added token to a list is a HEREDOC token.
 * 
 * This function checks the last added node to the provided `list` and verifies 
 * if its content represents a HEREDOC token. If the last node is not a HEREDOC
 * token, the function returns `false`, else, it returns `true`.
 * 
 * @param list A pointer to the list of tokens.
 * @return `true` if the last token in the list is of type `HEREDOC`, 
 *         otherwise `false`.
 */
bool	ft_is_heredoc_target(t_list **list)
{
	t_list	*last;
	t_token	*token;

	last = ft_lstlast(*list);
	if (!last || !last->content)
		return (false);
	token = (t_token *)last->content;
	if (token->type == HEREDOC)
		return (true);
	return (false);
}

/**
 * @brief Skips over a quoted substring in the input string.
 * 
 * This function identifies and skips the content enclosed by single or 
 * double quotes starting at the given index `i`. It checks for invalid 
 * wildcard usage (`'*'`) inside single quotes or in double quotes unless 
 * prefixed by a `$`. If such a wildcard is found, the function returns `-1` 
 * to indicate invalid input.
 * 
 * @param s The input string to analyze.
 * @param i The starting index to check for quoted substrings.
 * @return The index immediately after the closing quote, or `-1` if invalid 
 *         wildcard usage is detected.
 */
static int	ft_skip_quotes(char *s, int i)
{
	if (s[i] == SQUOTE)
	{
		i++;
		while (s[i] && s[i] != SQUOTE)
		{
			if (s[i] == '*') 
				return (-1);
			i++;
		}
		i++;
	}
	else if (s[i] == DQUOTE)
	{
		i++;
		while (s[i] && s[i] != DQUOTE)
		{
			 if (s[i] == '*' && s[i - 1] != '$') 
				return (-1);
			i++;
		}
		i++;
	}
	return (i);
}

/**
 * @brief Checks if a string contains a valid wildcard pattern.
 * 
 * This function evaluates whether the input string `s` contains a wildcard 
 * character (`'*'`) that is valid according to specific rules. Wildcards 
 * are considered invalid if:
 * - They appear inside single quotes.
 * - They appear inside double quotes without being prefixed by `$`.
 * - The string contains a `/` character, which invalidates wildcard patterns.
 * 
 * The function returns `true` if a valid wildcard is detected and `false` 
 * otherwise.
 * 
 * @param s The input string to analyze.
 * @return `true` if a valid wildcard is present, otherwise `false`.
 */
bool	ft_is_wildcard(char *s)
{
	int		i;
	bool	wildcard;

	i = 0;
	wildcard = false;
	while (s[i])
	{
		if (s[i] == SQUOTE || s[i] == DQUOTE)
		{
			i = ft_skip_quotes(s, i);
			if (i == -1)
				return (false);
		}
		else if (s[i] == '*' && s[i - 1] != '$')
		{
			wildcard = true;
			i++;
		}
		else if (s[i] == '/')
			return (false);
		else if (s[i])
			i++;
	}
	return (wildcard);
}
