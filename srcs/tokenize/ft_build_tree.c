#include "../../includes/minishell.h"

static bool	ft_find_next_pipe(t_list **list)
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
		{
			*list = (*list)->next; // update list to token after pipe '|' before returning
			return (true);
		}
		*list = (*list)->next;
	}
	return (false);
}

static void	ft_skip_export_tokens(t_list **list)
{
	t_token	*token;

	token = (*list)->content;
	while (*list && (token->type == EXPORT || token->type == EXPORT_AP))
	{
		if (!((*list)->next) 
				|| ((t_token *)(*list)->next->content)->type == PIPE)
			break ;
		else
		{
			*list = (*list)->next; //skip node
			token = (*list)->content;
		}
	}
}

void	*ft_build_tree(t_list **list)
{
	t_pipe	*pipe;

	ft_skip_export_tokens(list);
	pipe = (t_pipe *)malloc(sizeof(t_pipe));
	if (!pipe)
		return (NULL); //ft_error_hanlder(); malloc failed
	pipe->type = PIPE;
	pipe->left = ft_build_branch(list, NULL);
	if (!pipe->left)
		return (NULL);
	if (!list || !*list)
		return ((void *)pipe);
	if (ft_find_next_pipe(list)) // check if there is a pipe and if true, update list
		pipe->right = ft_build_tree(list);
	else
		pipe->right = NULL;
	return ((void *)pipe);
}
