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

// static bool	validate_open_parenthesis(char c, char current)
// {
// 	if (current == '<' || current == '>' || ft_isalnum(current))
// 	{
// 		printf(UNEXPECTED_TOKEN, PROG_NAME, c);
// 		return (false);
// 	}
// 	return (true);
// }

// static bool	validate_close_parenthesis(char c, bool left, char current)
// {
// 	if (left || current == '|')
// 	{
// 		printf(UNEXPECTED_TOKEN, PROG_NAME, c);
// 		return (false);
// 	}
// 	return (true);
// }

// static bool	validate_left_context(char *s, int i, bool left)
// {
// 	if (left && (s[i] == '|' || (s[i] == '.' && (ft_isspace(s[i + 1]) || s[i + 1] == ')'))))
// 	{
// 		printf(UNEXPECTED_TOKEN, PROG_NAME, s[i]);
// 		return (false);
// 	}
// 	return (true);
// }

// static int	handle_quotes_skip(char *s, int i, char *current, bool *left, bool *right)
// {
// 	*current = s[i];
// 	i = ft_find_next_quote(s, i, s[i]);
// 	*left = false;
// 	*right = false;
// 	return (i);
// }

// static bool	validate_character(char *s, int i, bool right, char *current)
// {
// 	if (right && s[i] != '|' && s[i] != '&')
// 	{
// 		printf(UNEXPECTED_TOKEN, PROG_NAME, ')');
// 		return (false);
// 	}
// 	*current = s[i];
// 	return (true);
// }

// static bool	validate_loop(char *s, int *i, bool *left, bool *right, char *current)
// {
// 	if (s[*i] == '(' && !validate_open_parenthesis(s[*i], *current))
// 		return (false);
// 	if (s[*i] == ')' && !validate_close_parenthesis(s[*i], *left, *current))
// 		return (false);
// 	if (!validate_left_context(s, *i, *left))
// 		return (false);
// 	if (s[*i] == DQUOTE || s[*i] == SQUOTE)
// 		*i = handle_quotes_skip(s, *i, current, left, right);
// 	else if (!ft_isspace(s[*i]) && !validate_character(s, *i, *right, current))
// 		return (false);
// 	*left = (s[*i] == '(');
// 	*right = (s[*i] == ')');
// 	return (true);
// }

// bool	ft_validate_parentheses(char *s)
// {
// 	int		i;
// 	bool	left;
// 	bool	right;
// 	char	current;

// 	if (!ft_count_parentheses(s))
// 		return (false);
// 	i = 0;
// 	left = false;
// 	right = false;
// 	current = ' ';
// 	while (s[i])
// 	{
// 		if (!validate_loop(s, &i, &left, &right, &current))
// 			return (false);
// 		i++;
// 	}
// 	if (left)
// 		printf(UNEXPECTED_TOKEN, PROG_NAME, '(');
// 	return (!left);
// }
