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
	if (!trim)
		return (0); //mensagem?
	specialchar = false;
	i = 0;
	if (ft_strchr(SPECIALCHARS, trim[i]))
	{
		if (trim[i] != '<' && trim[i] != '>')
		{
			printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]);
			return (free(trim), 0);
		}
	}
	while (trim[i])
	{
		if (trim[i] == 39 || trim[i] == 34)
		{
			i = ft_next_quote(s, i, trim[i]);
			if (i == -1)
			{
				printf("%s: open quotes are not supported\n", PROG_NAME); //ft_error_handler();
				return (0);
			}
			i++;
			specialchar = false;
			if (trim[i] == '\0') // check this
				break ;
		}
		if (ft_strchr(SPECIALCHARS, trim[i])) //check this with this
		{
			if (specialchar == true)
			{
				printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, trim[i]);
				free(trim);
				return (0);
			}
			if (trim[i] == '<' && trim[i + 1] == trim[i] && specialchar == false)
				i += 2;
			else if (trim[i] == '>' && trim[i + 1] == trim[i] && specialchar == false)
				i += 2;
			specialchar = true;
			if (trim[i] == '\0')
				break ;
			i++;
		}
		else if (!ft_isspace(trim[i]))
		{
			specialchar = false;
			i++;
		}
		if (ft_isspace(trim[i]))
			i++;
	}
	if (specialchar == true)
	{
		printf("%s: syntax error near unexpected token `%c'\n", PROG_NAME, s[i + 1]);
		return (free(trim), 0);
	}
	free(trim);
	return (1);
}
