#include "../../includes/minishell.h"

void	ft_process_quotes(t_token *token, t_state state)
{
	(void)token;
	(void)state;
	return ;
}

void	ft_process_token_list(t_list **list, char **my_envp)
{
	t_list	*current;
	t_token	*token;

	current = *list;
	while (current)
	{
		token = (t_token *)current->content;
		if (token->expand == true)
			ft_process_expansion(token, my_envp);
		current = current->next;
	}
}
