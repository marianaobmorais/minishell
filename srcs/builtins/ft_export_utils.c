#include "../../includes/minishell.h"

/* 
"export var" nao exporta uma variavel mais guarda o o nome da variavel que 
depois pode ser atribuida se for declarado var=texto apenas "export" imprime
todas as variaveis inclusive o que n\ao foi exportado e ordena por ordem
alfabetica de ASCII
o que pode ser feito para essa funcao eh ter no envp as variaveis que foram
declaradas mais nao exportadas, e o env apenas imprimir o que tiver '=' assim
nao seria necessario criar um novo array, a funcao history seria apenas para 
ordenar e imprimir todas inclusive as que nao tiverem '='
*/

void	ft_sort_str_tab(char **str, int n)
{
	int		k;
	int		j;
	char	*swap;

	k = 0;
	while (k < n - 1)
	{
		j = 0;
		while (j < n - k - 1)
		{
			if (ft_strcmp(str[j], str[j + 1]) > ft_strcmp(str[j + 1], str[j]))
			{
				swap = str[j];
				str[j] = str[j + 1];
				str[j + 1] = swap;
			}
			j++;
		}
		k++;
	}
}

void	ft_str_quotes(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	ft_putstr_fd("declare -x ", 1);
	while (str[i])
	{
		ft_putchar(str[i]);
		if (str[i] == '=' && flag == 0)
		{
			ft_putchar('"');
			flag++;
		}
		i++;
	}
	if (flag == 1)
		ft_putchar('"');
	ft_putchar('\n');
}

void	ft_print_export(char **envp)
{
	char	**sorted_envp;
	int		size_envp;
	int		i;

	sorted_envp = ft_get_my_envp(envp);
	size_envp = 0;
	i = 0;
	while (envp[size_envp])
		size_envp++;
	ft_sort_str_tab(sorted_envp, size_envp);
	while (sorted_envp[i])
	{
		ft_str_quotes(sorted_envp[i]);
		free(sorted_envp[i]);
		i++;
	}
	free(sorted_envp);
}
