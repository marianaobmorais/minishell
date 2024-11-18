#include "../../includes/minishell.h"

static void	ft_assign_redir_mode(t_redir **redir)
{
	//do we need this function?
	if ((*redir)->type == OUTFILE) 
	{
		(*redir)->mode = O_WRONLY | O_CREAT | O_TRUNC;
		(*redir)->fd = 1;
	}
	else if ((*redir)->type == INFILE)
	{
		(*redir)->mode = O_RDONLY;
		(*redir)->fd = 0;
	}
	else if ((*redir)->type == APPEND)
	{
		(*redir)->mode = O_WRONLY | O_CREAT | O_APPEND;
		(*redir)->fd = 1;
	}
	else if ((*redir)->type == HEREDOC)
	{
		(*redir)->mode = O_RDONLY; //not sure??
		(*redir)->fd = 0; // not sure??
	}
}

static t_redir	*ft_init_redir(t_token *token, t_list **list)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = token->type;
	ft_assign_redir_mode(&redir); //precisamos disso?
	if ((*list)->next && (*list)->next->content)
	{
		*list = (*list)->next; // move up to target
		redir->target = (t_token *)(*list)->content;
	}
	return (redir);
}

static t_redir	*ft_create_redir_node(t_token *token, t_list **list, t_exec *exec)
{
	t_redir	*redir;
	t_list	*list_next;

	redir = ft_init_redir(token, list);
	if (!redir)
		return (NULL); //ft_error_hanlder(); malloc failed
	if (!(*list)->next || ((t_token *)(*list)->next->content == PIPE)) //check whether next is not NULL or PIPE
		redir->next =  (void *)exec;
	else
	{
		*list = (*list)->next; // move up to next node
		list_next = *list;
		if (!exec && ((t_token *)(*list)->content)->type == EXEC)
			redir->next = ft_build_branch(&list_next, exec);
		else if (ft_find_next_redir(&list_next)) // update pointer to next redir before pipe
			redir->next = ft_build_branch(&list_next, exec);
		else
			redir->next = (void *)exec; //whether it's is NULL or not
	}
	return (redir);
}

static t_exec	*ft_create_exec_node(t_token *token, t_list **list)
{
	t_exec	*exec;

	exec = (t_exec *)malloc(sizeof(t_exec));
	if (!exec)
		return (NULL); //ft_error_hanlder(); malloc failed
	exec->type = token->type;
	exec->pathname = token->value;
	exec->args = ft_get_args(list);
	token = (*list)->content;
	while (*list && (token->type == EXEC || token->type == EXPORT
			|| token->type == EXPORT_AP)) // looking for pipe '|', redirs or NULL
	{
		token = (t_token *)(*list)->content;
		if (!(token->type == EXEC || token->type == EXPORT
				|| token->type == EXPORT_AP))
			break ;
		*list = (*list)->next;
	}
	return (exec);
}

void	*ft_build_branch(t_list **list, t_exec *exec)
{
	t_token	*token;
	t_redir	*redir;

	token = (*list)->content;
	if (!exec && (token->type == EXEC || token->type == EXPORT
			|| token->type == EXPORT_AP))
	{
		exec = ft_create_exec_node(token, list);
		if (!list || !*list || !exec)
			return ((void *)exec);
		token = (*list)->content;
		if (token->type == PIPE)
			return ((void *)exec);
	}
	if (token->type == OUTFILE || token->type == INFILE
			|| token->type == APPEND || token->type == HEREDOC)
	{
		redir = ft_create_redir_node(token, list, exec);
		return ((void *)redir);
	}
	return (NULL);
}
