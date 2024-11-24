#include "../../includes/minishell.h"

bool ft_validate_parentheses(char *s)
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
	c = ' ';
	while (s[i])
	{
		if (s[i] == '(')
		{
			if (c == '<' || c == '>' || ft_isalnum(c))
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]), false); //ft_error_handler(); 258
			left = true;
		}
		else if (s[i] == ')')
		{
			if (left || c == '|')
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]), false); //ft_error_handler(); 258
			right = true;
		}
		else if (left && s[i] == '|')
			return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]), false); //ft_error_handler(); 258
		else if (s[i] == DQUOTE || s[i] == SQUOTE)
		{
			c = s[i];
			i = ft_find_next_quote(s, i, s[i]);
			left = false;
			right = false;
		}
		else if (!ft_isspace(s[i]))
		{
			if (right && s[i] != '|' && s[i] != '&')
				return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, ')'), false); //ft_error_handler(); 258
			c = s[i];
			left = false;
			right = false;
		}
		i++;
	}
	if (left)
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, '('), false); //ft_error_handler(); 258
	return (true);
}
