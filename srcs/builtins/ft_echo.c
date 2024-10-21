#include "../../includes/minishell.h"

void	ft_echo(char *args)
{
	char	**str;
	bool	flag;

	flag = false;
	//se tiver flag -n? -nnnn? -n -nnn -n?
	if (args == NULL) // sem argumento
	{
		printf("\n");
		return ;
	}
	str = ft_split_quotes(args, 32);
	if (!str)
		return ;
	int i = 0;
	while (str[i])
	{
		printf("%s", str[i]);
		if (str[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	return ;
}