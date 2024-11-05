#include "../../includes/minishell.h"

int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int	ft_check_syntax(char *s)
{
	char	*trim;
	int		i;
	bool	specialchar;

	trim = ft_strtrim(s, ISSPACE);

	printf("%s", trim);
	
	specialchar = false;
	i = 0;
	if (ft_strchr(SPECIALCHARS, s[i]))
	{
		if (s[i] != '<' && s[i] != '>')
		{
			printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]);
			return (0); // add message
		}
	}
	while (s[i])
	{
		if (s[i] == 39 || s[i] == 34)
		{
			i = ft_next_quote(s, i, s[i]);
			if (i == -1)
			{
				printf("%s: open quotes are not supported\n", PROG_NAME); //ft_error_handler();
				return (0);
			}
			i++;
			specialchar = false;
		}
		if (ft_strchr(SPECIALCHARS, s[i]))
		{
			if (specialchar == true)
			{
				printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i]);
				return (0);
			}
			if (s[i] == '<' && s[i + 1] == s[i] && specialchar == false)
				i += 2;
			else if (s[i] == '>' && s[i + 1] == s[i] && specialchar == false)
				i += 2;
			specialchar = true;
			i++;
		}
		else if (!ft_isspace(s[i]))
		{
			specialchar = false;
			i++;
		}
		if (ft_isspace(s[i]))
			i++;
	}
	//if (specialchar == true)
		//printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i - 1]);
	return (1);
}
