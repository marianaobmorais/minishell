#include "../../includes/minishell.h"

int	ft_next_quote(char *s, int i, char c)
{
	i++;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}
