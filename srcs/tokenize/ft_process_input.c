#include "../../includes/minishell.h"


void	ft_print_list(t_list **token_list)
{
	t_list	*current;
	t_token	*token;

	if (!token_list || !*token_list)
	{
		printf("List is empty\n");
		return ;
	}
	printf("----------------------------------------------------\n");
	printf("| %-13s | %-8s | %-10s | %-10s |\n", "token", "type", "state", "expansion");
	printf("----------------------------------------------------\n");
	current = *token_list;
	while (current)
	{
		token = (t_token *)current->content;
		printf("| %-13s | %-8i | %-10i | %-10i |\n",
			token->value,
			token->type,
			token->state,
			token->expand);
		current = current->next;
	}
	printf("-----------------------------------------------------\n");
}

void	ft_free_list(t_list **list)
{
	ft_lstclear(list, free);
	if (list)
		free(list);
	list = NULL;
}

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
	ft_print_list(token_list); // delete later
	//treat expansions and quotes
	//ft_parse_token_list(token_list, my_envp);
	//create tree
	ft_free_list(token_list);
}
