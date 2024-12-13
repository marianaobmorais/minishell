#include "../includes/minishell.h"

/**
 * @brief Copies the attributes of a token and appends it to a new list.
 *
 * This function creates a new token with the same properties as the source
 * token and appends it to the provided list.
 *
 * @param new_token A pointer to the newly allocated token to populate.
 * @param tmp The current node in the source list being copied.
 * @param new A pointer to the head of the destination list to which the copied
 *        node is added.
 * @return `true` on successful creation and addition of the new node; `false`
 *         otherwise.
 */
static bool	ft_copy_token(t_token *new_token, t_list *tmp, t_list **new)
{
	t_list	*new_node;

	new_token->type = ((t_token *)tmp->content)->type;
	new_token->state = ((t_token *)tmp->content)->state;
	new_token->expand = ((t_token *)tmp->content)->expand;
	new_node = ft_lstnew((t_token *)new_token);
	if (!new_node)
		return (ft_error_malloc("new_node"), false);
	ft_lstadd_back(new, new_node);
	return (true);
}

/**
 * @brief Creates a deep copy of a linked list of tokens.
 *
 * This function duplicates a token list, creating new tokens and nodes for
 * each entry in the source list.
 *
 * @param old A pointer to the head of the source list to copy.
 * @return A pointer to the head of the newly created list, or `NULL` if an
 *         error occurs.
 */
t_list	**ft_copy_list(t_list **old)
{
	t_list	**new;
	t_list	*tmp;
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
			return (ft_free_list(*new), free(new), NULL);
		new_token->value = ft_strdup(((t_token *)tmp->content)->value);
		if (!new_token->value)
			return (ft_free_list(*new), free(new),
				ft_error_malloc("new_token->value"), NULL);
		if (!ft_copy_token(new_token, tmp, new))
			return (ft_free_list(*new), free(new), NULL);
		tmp = tmp->next;
	}
	return (new);
}
