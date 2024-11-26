#include "../../includes/minishell.h"

static bool	ft_count_parentheses(char *s)
{
	int	i; //write brief
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
				return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 2
			count--;
		}
		else if (s[i] == DQUOTE || s[i] == SQUOTE)
		{
			i = ft_find_next_quote(s, i, s[i]);
			if (i == -1)
				return (printf(OPEN_QUOTE, PROG_NAME), false); //ft_error_handler(); 2
		}
	}
	if (count == 0)
		return (true);
	return (printf(OPEN_PRTHESES, PROG_NAME), false); //ft_error_handler(); 2
}

bool	ft_validate_parentheses(char *s)
{
	int		i;
	bool	left;
	bool	right;
	char	last;

	if (!ft_count_parentheses(s))
		return (false);
	left = false;
	right = false;
	last = ' ';
	i = -1;
	while (s[++i])
	{
		if (s[i] == '(' && !ft_validate_left(s[i], last, &left))
			return (false);
		else if (s[i] == ')' && !ft_validade_right(s[i], last, &left, &right))
			return (false);
		else if (!ft_validate_left_context(s, i, &last, &left))
			return (false);
		else if (!ft_handle_chars(s, &i, &left, &right))
			return (false);
	}
	if (left)
		return (printf(UNEXPECTED_TOKEN, PROG_NAME, '('), false); //ft_error_handler(); 2
	return (true);
}


// bool	ft_validate_parentheses(char *s)
// {
// 	//write brief
// 	int		i;
// 	bool	left;
// 	bool	right;
// 	char	curr;
// 	if (!ft_count_parentheses(s))
// 		return (false);
// 	i = 0;
// 	left = false;
// 	right = false;
// 	curr = ' ';
// 	while (s[i])
// 	{
// 		if (s[i] == '(')
// 		{
// 			if (curr == '<' || curr == '>' || ft_isalnum(curr))
// 				return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 2
// 			left = true;
// 		}
// 		else if (s[i] == ')')
// 		{
// 			if (left || curr == '|')
// 				return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 2
// 			right = true;
// 		}
// 		else if (left && (s[i] == '|' || (s[i] == '.' && (ft_isspace(s[i + 1]) || s[i + 1] == ')'))))
// 			return (printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]), false); //ft_error_handler(); 2
// 		else if (s[i] == DQUOTE || s[i] == SQUOTE)
// 		{
// 			curr = s[i];
// 			i = ft_find_next_quote(s, i, s[i]);
// 			left = false;
// 			right = false;
// 		}
// 		else if (!ft_isspace(s[i]))
// 		{
// 			if (right && s[i] != '|' && s[i] != '&')
// 				return (printf(UNEXPECTED_TOKEN, PROG_NAME, ')'), false); //ft_error_handler(); 2
// 			curr = s[i];
// 			left = false;
// 			right = false;
// 		}
// 		i++;
// 	}
// 	if (left)
// 		return (printf(UNEXPECTED_TOKEN, PROG_NAME, '('), false); //ft_error_handler(); 2
// 	return (true);
// }
