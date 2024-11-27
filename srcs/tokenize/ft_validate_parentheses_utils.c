#include "../../includes/minishell.h"

bool	ft_validate_left(char c, char last, bool *left)
{
	if (last == '<' || last == '>' || ft_isalnum(last))
		return (ft_stderror(FALSE, UNEXPECTED_TOKEN, c), ft_exit_status(2, TRUE, FALSE), false);
	*left = true;
	return (true);
}

bool	ft_validade_right(char c, char last, bool *left, bool *right)
{
	if (*left || last == '|')
		return (ft_stderror(FALSE, UNEXPECTED_TOKEN, c), ft_exit_status(2, TRUE, FALSE), false);
	*right = true;
	return (true);
}

bool	ft_validate_left_context(char *s, int i, bool *left)
{
	if (*left && (s[i] == '|'
			|| (s[i] == '.' && (ft_isspace(s[i + 1]) || s[i + 1] == ')'))))
		return (ft_stderror(FALSE, UNEXPECTED_TOKEN, s[i]), ft_exit_status(2, TRUE, FALSE), false);
	return (true);
}

bool	ft_handle_chars(char *s, int *i, bool *left, bool *right, char *last)
{
	//too many args
	if (s[*i] != '(' && s[*i] != ')' && !ft_isspace(s[*i]))
	{
		if (*right && !ft_strchr(METACHARS, s[*i]))
			return (ft_stderror(FALSE, UNEXPECTED_TOKEN, ')'), ft_exit_status(2, TRUE, FALSE), false);
		*last = s[*i];
		if (s[*i] == DQUOTE || s[*i] == SQUOTE)
			*i = ft_find_next_quote(s, *i, s[*i]);
		*left = false;
		*right = false;
	}
	return (true);
}
