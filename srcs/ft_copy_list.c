#include "../includes/minishell.h"


// t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
// {
// 	t_list	*new_lst;
// 	t_list	*new_node;
// 	void	*new_content;

// 	if (lst == NULL || f == NULL || del == NULL)
// 		return (NULL);
// 	new_lst = NULL;
// 	while (lst != NULL)
// 	{
// 		new_content = (*f)(lst->content);
// 		if (new_content != NULL)
// 			new_node = ft_lstnew(new_content);
// 		if (new_node == NULL || new_content == NULL)
// 		{
// 			ft_lstclear(&new_lst, del);
// 			(*del)(new_content);
// 			return (NULL);
// 		}
// 		ft_lstadd_back(&new_lst, new_node);
// 		lst = lst->next;
// 	}
// 	return (new_lst);
// }

t_list	**ft_copy_list(t_list **old)
{
	t_list	**new; //write brief
	t_list	*tmp;
	t_list	*new_node;
	t_token	*new_token;

	new = (t_list **)malloc(sizeof(t_list *)); //maybe I can malloc in ft_isjustbuiltin
	if (!new)
		ft_error_malloc("new");
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
