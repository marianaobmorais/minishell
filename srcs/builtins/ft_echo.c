#include "../../includes/minishell.h"

bool	ft_arg_is_flag(char *s)
{
	int	i;

	if (!(s[0] == '-' && s[1] == 'n')) // quais são as outras flags? aceitar como string ou mandar mensagem de erro?
		return (false);
	else
	{
		i = 2;
		while (s[i] == 'n')
			i++;
		if (s[i] == '\0')
			return (true);
		return (false);
	}
}

void	ft_echo(char *args)
{
	char	**s;
	bool	flag;
	int		i;

	flag = false;
	if (args == NULL) // sem argumento
	{
		printf("\n");
		return ;
	}
	s = ft_split_quotes(args, ' ');
	if (!s)
		return ;
	i = 0;
	while (ft_arg_is_flag(s[i])) // &flag will work?
	{
		flag = true;
		i++;
	}
	while (s[i])
	{
		printf("%s", s[i]);
		if (s[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	return ;
}
