#include "../../includes/minishell.h"

bool	ft_validate_left(char c, char last, bool *left)
{
	if (last == '<' || last == '>' || ft_isalnum(last))
		return (printf(UNEXPECTED_TOKEN, PROG_NAME, c), false); //ft_error_handler(); 2
	*left = true;
	return (true);
}

bool	ft_validade_right(char c, char last, bool *left, bool *right)
{
	if (*left || last == '|')
		return (printf(UNEXPECTED_TOKEN, PROG_NAME, c), false); //ft_error_handler(); 2
	*right = true;
	return (true);
}

bool	ft_validate_left_context(char *s, int i, bool *left)
{
	if (*left && (s[i] == '|'
			|| (s[i] == '.' && (ft_isspace(s[i + 1]) || s[i + 1] == ')'))))
		return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 2
	return (true);
}

bool	ft_handle_chars(char *s, int *i, bool *left, bool *right, char *last)
{
	//to many args
	if (s[*i] != '(' && s[*i] != ')' && !ft_isspace(s[*i]))
	{
		if (*right && !ft_strchr(METACHARS, s[*i]))
			return (printf(UNEXPECTED_TOKEN, PROG_NAME, ')'), false);
		*last = s[*i];
		if (s[*i] == DQUOTE || s[*i] == SQUOTE)
			*i = ft_find_next_quote(s, *i, s[*i]);
		*left = false;
		*right = false;
	}
	return (true);
}
