#include "../../includes/minishell.h"

bool	ft_handle_paretheses(char c, char *last, bool *left, bool *right)
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

bool	ft_validate_left_context(char *s, int i, bool *left)
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
