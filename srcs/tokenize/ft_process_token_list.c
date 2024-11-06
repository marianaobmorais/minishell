#include "../../includes/minishell.h"

void	ft_remove_quotes(t_token *token)
{
	char	quote;
	int		i;

	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == SQUOTE || token->value[i] == DQUOTE)
		{
			quote = token->value[i];
			ft_memmove(&token->value[i], &token->value[i + 1], ft_strlen(&token->value[i + 1]) + 1);
			while (token->value[i] && token->value[i] != quote)
				i++;
			if (token->value[i] == quote)
				ft_memmove(&token->value[i], &token->value[i + 1], ft_strlen(&token->value[i + 1]) + 1);
		}
		else
			i++;
	}
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
			ft_expand_tokens(token, my_envp);
		if (token->state == IN_QUOTE)
			ft_remove_quotes(token);
		current = current->next;
	}
}
