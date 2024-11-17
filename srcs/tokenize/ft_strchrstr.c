#include "../../includes/minishell.h"

static int	ft_inset(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		++set;
	}
	return (0);
}

int	ft_strchrstr(const char *s1, const char *s2)
{
	while (*s1 != '\0' && !ft_inset(*s1, s2))
		++s1;
	if (*s1 != '\0')
		return (1);
	return (0);
}