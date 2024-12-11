#include "../includes/minishell.h"

t_list	**ft_copy_list(t_list **old)
{
	t_list	**new; //write brief adn refactor
	t_list	*tmp;
	t_list	*new_node;
	t_token	*new_token;

	new = (t_list **)malloc(sizeof(t_list *));
	if (!new)
		ft_error_malloc("new");
	*new = NULL;
	tmp = *old;
	while (tmp)
	{
		new_token = (t_token *)malloc(sizeof(t_token));
		if (!new_token)
			return (ft_error_malloc("new_token"), NULL);
		new_token->value = ft_strdup(((t_token *)tmp->content)->value);
		if (!new_token->value)
			return (free(new_token), ft_error_malloc("new_token->value"), NULL);
		new_token->type = ((t_token *)tmp->content)->type;
		new_token->state = ((t_token *)tmp->content)->state;
		new_token->wildcard = ((t_token *)tmp->content)->wildcard;
		new_token->expand = ((t_token *)tmp->content)->expand;
		new_node = ft_lstnew((t_token *)new_token);
		if (!new_node)
			return (free(new_token->value), free(new_token),
				ft_error_malloc("new_node"), NULL);
		ft_lstadd_back(new, new_node);
		tmp = tmp->next;
	}
	return (new);
}
