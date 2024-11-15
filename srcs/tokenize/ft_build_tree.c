#include "../../includes/minishell.h"

char	**ft_add_to_vector(char **vector, char *new_str)
{
	char	**res;
	int		i;

	i = 0;
	if (vector)
		while (vector[i])
			i++;
	res = (char **)malloc(sizeof(char *) * (i + 2)); // + 2: one for new string and one for NULL
	if (!res)
		return (NULL); 
	i = 0;
	if (!vector)
		res[i++] = ft_strdup(new_str);
	else
	{
		while (vector[i])
		{
			res[i] = ft_strdup(vector[i]);
			i++;
		}
		res[i++] = ft_strdup(new_str);
		ft_free_vector(vector);
	}
	res[i] = NULL;
	return (res);
}

char	**ft_get_args(t_list **list)
{
	char	**args;
	t_list	*current;
	t_token	*token;

	args = NULL;
	current = *list;
	while (current)
	{
		token = (t_token *)current->content;
		if (token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP)
			args = ft_add_to_vector(args, token->value);
		else if (token->type == APPEND || token->type == OUTFILE || token->type == HEREDOC || token->type == INFILE)
		{
			if (current->next && ((t_token *)current->next->content)->type != PIPE)
				current = current->next;
		}
		else if (token->type == PIPE)
			break ;
		current = current->next;
	}
	return (args);
}

bool	ft_find_next_pipe(t_list **list)
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

bool	ft_find_next_redir(t_list **list)
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
			break ;
		if (token->type == OUTFILE || token->type == INFILE || token->type == APPEND || token->type == HEREDOC)
			return (true);
		*list = (*list)->next;
	}
	return (false);
}

bool	ft_find_next_exec(t_list **list)
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
			break ;
		if (token->type == EXEC)
			return (true);
		*list = (*list)->next;
	}
	return (false);
}

void	*ft_build_branch(t_list **list, t_exec *exec)
{
	t_list	*list_next;
	t_token	*token;
	t_redir	*redir;

	token = (*list)->content;
	if (!exec && (token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP))
	{
		exec = (t_exec *)malloc(sizeof(t_exec));
		if (!exec)
			return (NULL); //ft_error_hanlder(); malloc failed
		exec->type = token->type;
		exec->pathname = token->value;
		exec->args = ft_get_args(list);
		token = (*list)->content;
		while (*list && (token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP)) // looking for pipe '|', redirs or NULL
		{
			token = (*list)->content;
			if (!(token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP))
				break ;
			*list = (*list)->next;
		}
		if (!list || !*list)
			return ((void *)exec);
		if (token->type == PIPE)
			return ((void *)exec);
	}
	//if there are redirs
	if (token->type == OUTFILE || token->type == INFILE || token->type == APPEND || token->type == HEREDOC)
	{
		redir = (t_redir *)malloc(sizeof(t_redir));
		if (!redir)
			return (NULL); //ft_error_hanlder(); malloc failed
		redir->type = token->type;
		if ((*list)->next && (*list)->next->content)
		{
			*list = (*list)->next; // move up to target
			//if (((t_token *)(*list)->content)->type == PIPE)
			//	printf("how to execute >|?\n");//
			//else
			redir->target = (t_token *)(*list)->content;
		}
		if (!(*list)->next || ((t_token *)(*list)->next->content == PIPE)) //check whether next is not NULL or PIPE
			return ((void *)redir);
		*list = (*list)->next; // move up to next node
		list_next = *list;
		if (!exec && ((t_token *)(*list)->content)->type == EXEC)
			redir->next = ft_build_branch(&list_next, exec);
		else if (ft_find_next_redir(&list_next)) // update pointer to next redir before pipe
			redir->next = ft_build_branch(&list_next, exec);
		else
		{
			if (exec)
				redir->next = (void *)exec;
			else
				redir->next = NULL;
		}
		return ((void *)redir);
	}
	return (NULL);
}

void	*ft_build_tree(t_list **list)
{
	t_token	*token;
	t_pipe	*pipe;

	token = (*list)->content;
	while (*list && (token->type == EXPORT || token->type == EXPORT_AP))
	{
		if (!((*list)->next) || ((t_token *)(*list)->next->content)->type == PIPE)
			break ;
		else
		{
			*list = (*list)->next; //skip node
			token = (*list)->content;
		}
	}
	pipe = (t_pipe *)malloc(sizeof(t_pipe));
	if (!pipe)
		return (NULL); //ft_error_hanlder(); malloc failed
	pipe->type = PIPE;
	pipe->left = ft_build_branch(list, NULL);
	if (!pipe->left)
		return (NULL);
	if (!list || !*list)
		return ((void *)pipe);
	if (ft_find_next_pipe(list)) // check is there is pipe and if true, update list_to_the_right
		pipe->right = ft_build_tree(list);
	else
		pipe->right = NULL;
	return ((void *)pipe);
}
