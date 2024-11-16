#include "../../includes/minishell.h"

void	ft_assign_redir_mode(t_redir **redir) //precisamos disso?
{
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
		(*redir)->mode = O_RDONLY; //not sure?? //O_WRONLY | O_CREAT | O_APPEND;
		(*redir)->fd = 0; // not sure??
	}
}

t_exec	*ft_create_exec_node(t_token *token, t_list **list)
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

t_redir	*ft_create_redir_node(t_token *token, t_list **list, t_exec *exec)
{
	t_redir	*redir;
	t_list	*list_next;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL); //ft_error_hanlder(); malloc failed
	redir->type = token->type;
	ft_assign_redir_mode(&redir); //precisamos disso?
	if ((*list)->next && (*list)->next->content)
	{
		*list = (*list)->next; // move up to target
		redir->target = (t_token *)(*list)->content;
	}
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

void	*ft_build_branch(t_list **list, t_exec *exec)
{
	t_token	*token;
	t_redir	*redir;

	token = (*list)->content;
	if (!exec && (token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP))
	{
		exec = ft_create_exec_node(token, list);
		if (!list || !*list || !exec)
			return ((void *)exec);
		token = (*list)->content;
		if (token->type == PIPE)
			return ((void *)exec);
	}
	if (token->type == OUTFILE || token->type == INFILE || token->type == APPEND || token->type == HEREDOC)
	{
		redir = ft_create_redir_node(token, list, exec);
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
	//start root with pipe struct, regardless of the presence of pipes
	pipe = (t_pipe *)malloc(sizeof(t_pipe));
	if (!pipe)
		return (NULL); //ft_error_hanlder(); malloc failed
	pipe->type = PIPE;
	pipe->left = ft_build_branch(list, NULL);
	if (!pipe->left)
		return (NULL);
	if (!list || !*list)
		return ((void *)pipe);
	//printf("tree: checking content for right pipe: %s\n", ((t_token *)(*list)->content)->value); //debug
	if (ft_find_next_pipe(list)) // check is there is pipe and if true, update list_to_the_right
	{
		//printf("tree: calling ft_build_tree\n"); //debug
		pipe->right = ft_build_tree(list);
	}
	else
		pipe->right = NULL;
	return ((void *)pipe);
}

// void	*ft_build_branch(t_list **list, t_exec *exec) //before refactoring
// {
// 	t_list	*list_next;
// 	t_token	*token;
// 	t_redir	*redir;
// 	token = (*list)->content;
// 	if (!exec && (token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP))
// 	{
// 		//printf("ft_build_branch: exec\n"); //debug
// 		exec = (t_exec *)malloc(sizeof(t_exec));
// 		if (!exec)
// 			return (NULL); //ft_error_hanlder(); malloc failed
// 		exec->type = token->type;
// 		exec->pathname = token->value;
// 		exec->args = ft_get_args(list);
// 		token = (*list)->content;
// 		while (*list && (token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP)) // looking for pipe '|', redirs or NULL
// 		{
// 			token = (*list)->content;
// 			//printf("exec loop: Current token: %s\n", token->value); // Debug
// 			if (!(token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP))
// 				break ;
// 			*list = (*list)->next;
// 		}
// 		if (!list || !*list)
// 		{
// 			//printf("exec branch: found null, returning t_exec to pipe->left\n"); //debug
// 			return ((void *)exec);
// 		}
// 		if (token->type == PIPE)
// 		{
// 			//printf("exec branch: found a pipe\n"); //debug
// 			return ((void *)exec);
// 		}
// 		//else
// 		//	printf("exec branch: Found redir: %s\n", (*list) ? ((t_token *)(*list)->content)->value : "NULL"); // debug
// 	}
// 	//if there are redirs
// 	if (token->type == OUTFILE || token->type == INFILE || token->type == APPEND || token->type == HEREDOC)
// 	{
// 		//printf("ft_build_branch: redir\n"); //debug
// 		redir = (t_redir *)malloc(sizeof(t_redir));
// 		if (!redir)
// 			return (NULL); //ft_error_hanlder(); malloc failed
// 		redir->type = token->type;
// 		ft_assign_redir_mode(&redir);
// 		if ((*list)->next && (*list)->next->content)
// 		{
// 			*list = (*list)->next; // move up to target
// 			//if (((t_token *)(*list)->content)->type == PIPE)
// 			//	printf("how to execute >|?\n");//
// 			//else
// 			redir->target = (t_token *)(*list)->content;
// 			//printf("redir branch: target: %s\n", ((t_token *)(*list)->content)->value); //debug
// 		}
// 		if (!(*list)->next || ((t_token *)(*list)->next->content == PIPE)) //check whether next is not NULL or PIPE
// 		{
// 			if (exec)
// 				redir->next = exec;
// 			return ((void *)redir);
// 		}
// 		*list = (*list)->next; // move up to next node
// 		list_next = *list;
// 		if (!exec && ((t_token *)(*list)->content)->type == EXEC)
// 			redir->next = ft_build_branch(&list_next, exec);
// 		else if (ft_find_next_redir(&list_next)) // update pointer to next redir before pipe
// 			redir->next = ft_build_branch(&list_next, exec);
// 		else
// 		{
// 			if (exec)
// 			{
// 				//printf("redir branch: next is exec\n"); //debug
// 				redir->next = (void *)exec;
// 			}
// 			else
// 			{
// 				//printf("redir branch: next is null\n"); //debug
// 				redir->next = NULL;
// 			}
// 		}
// 		//printf("redir branch: returning t_redir to pipe->left\n"); //debug
// 		return ((void *)redir);
// 	}
// 	return (NULL);
// }