#include "../../includes/minishell.h"

int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

static int	ft_iterate_str(char *trim, int i, bool *special)
{
	if (trim[i] == 39 || trim[i] == 34)
	{
		i = ft_next_quote(trim, i, trim[i]);
		if (i == -1)
			return (printf("%s: open quotes are not supported\n", PROG_NAME), -1); //ft_error_handler();
		*special = false;
	}
	if (ft_strchr(SPECIALCHARS, trim[i]))
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
	if (ft_strchr(METACHARS, trim[i]))
	{
		if (*special == true)
			return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]), -1); //ft_error_handler();
		*special = true;
		if ((trim[i] == '<' || trim[i] == '>') && trim[i + 1] == trim[i])
			i += 1;
	}
	return (i);
}

static bool	ft_first_char(char c)
{
	if (ft_strchr(SPECIALCHARS, c) || ft_strchr(METACHARS, c))
	{
		if (c != '<' && c != '>')
		{
			printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, c);  //ft_error_handler();
			return (true);
		}
	}
	return (false);
}

int	ft_check_syntax(char *s)
{
	char	*trim;
	int		i;
	bool	special;

	trim = ft_strtrim(s, ISSPACE);
	if (!trim)
		return (0); //ft_error_handler();
	i = 0;
	special = ft_first_char(trim[i]);
	if (special)
		return (free(trim), 0);
	while (trim[i])
	{
		i = ft_iterate_str(trim, i, &special);
		if (i == -1)
			return (free(trim), 0);
		if (trim[i] && !ft_isspace(trim[i]) && !ft_strchr(METACHARS, trim[i]))
			special = false;
		if (trim[i])
			i++;
	}
	if (special == true)
		return (printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i - 1]), free(trim), 0); //ft_error_handler();
	return (free(trim), 1);
}

