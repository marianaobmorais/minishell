#include "../../includes/minishell.h"

void	ft_process_input(char *input, char **my_envp)
{
	(void)my_envp;
	t_list	**token_list;
	char	*trimmed;

	if (!ft_check_syntax(input))
		return ;
	trimmed = ft_strtrim(input, ISSPACE);
	token_list = ft_create_token_list(trimmed);
	if (!token_list)
		return ;
}
