#include "../../includes/minishell.h"

static bool ft_count_parentheses(char *s)
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
				return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 258
			count--;
		}
		else if (s[i] == DQUOTE || s[i] == SQUOTE)
		{
			i = ft_find_next_quote(s, i, s[i]);
			if (i == -1)
				return (printf(OPEN_QUOTE, PROG_NAME), false); //ft_error_handler(); 2
		}
		i++;
	}
	if (count == 0)
		return (true);
	return (printf(OPEN_PRTHESES, PROG_NAME), false); //ft_error_handler(); 2
 
}

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
				return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 258
			left = true;
		}
		else if (s[i] == ')')
		{
			if (left || c == '|')
				return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 258
			right = true;
		}
		else if (left && (s[i] == '|' || (s[i] == '.' && (ft_isspace(s[i + 1]) || s[i + 1] == ')'))))
			return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 258
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
				return (printf(UNEXPECTED_TOKEN, PROG_NAME, ')'), false); //ft_error_handler(); 258
			c = s[i];
			left = false;
			right = false;
		}
		i++;
	}
	if (left)
		return (printf(UNEXPECTED_TOKEN, PROG_NAME, '('), false); //ft_error_handler(); 258
	return (true);
}
