#include "../includes/minishell.h"

char	*ft_charjoin(char *str, char c)
{
	size_t	len;
	size_t	i;
	char	*res;


	len = ft_strlen(str);
	res = (char *)malloc(sizeof(char) * (len + 2));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		++i;
	}
	res[i] = c;
	res[++i] = '\0';
	if (str)
		free(str);
	return (res);
}