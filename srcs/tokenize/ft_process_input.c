#include "../../includes/minishell.h"


t_type	ft_get_node_type(char *str)
{
	//t_type	type;

	if (str[0] == '|')
		return (PIPE);
	else
		return (BUILTIN);
}

void	ft_process_input(char *input, char **my_envp)
{
	(void)my_envp;
	//t_list	*token_list;
	//t_token	*new_node;

	if (!ft_check_syntax(input))
		return ;
	printf("syntax is ok\n");
	//token_list = ft_create_list();
}

/* void	ft_process_input(char *input, char **my_envp)
{
	(void)my_envp;
	char	**args;
	int		i;
	t_list	**token_list;
	t_token	*new_node;

	args = ft_split_argv(input, ' ');
	if (args == NULL)
		return ;

	i = 0;
	while (args[i])
	{
		new_node->value = args[i];
		if (ft_strchr(METACHARS, args[i][0]))
			new_node->type = ft_get_node_type(args[i]);
		else
			new_node->type = EXEC;

		i++;
	}


	i = 0;
	while (args[i])
	{
		printf("token %d: %s\n", i + 1, args[i]);
		i++;
	}
} */