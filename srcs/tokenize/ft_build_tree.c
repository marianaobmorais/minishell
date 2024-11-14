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
		if (token->type == EXEC)
			args = ft_add_to_vector(args, token->value);
		else if (token->type == APPEND || token->type == OUTFILE || token->type == HEREDOC || token->type == INFILE)
		{
			if (current->next && ((t_token *)current->next->content)->type != PIPE) //the check current->next is redundant
				current = current->next;
		}
		else if (token->type == PIPE)
			break ;
		current = current->next;
	}
	return (args);
}

void	*ft_build_tree(t_list **list)
{
	t_token	*token;
	t_pipe	*pipe;
	t_redir	*redir;
	t_exec	*exec;

	if (!list || !*list)
	{
		printf("List is empty\n"); //delete later
		return (NULL);
	}
	token = (*list)->content;
	if (token->type == EXPORT || token->type == EXPORT_AP)
	{
		if (!((*list)->next))
			printf("save in local variable\n"); //need to do this
		else if (((t_token *)(*list)->next->content)->type == PIPE)
			printf("don't save in local variable or do it in child process\n"); //need to do this
		else
			*list = (*list)->next; //skip it
	}
	if (token->type == PIPE)
	{
		pipe = (t_pipe *)malloc(sizeof(t_pipe));
		if (!pipe)
			return (NULL); //ft_error_hanlder(); malloc failed
		pipe->token = token;
		*list = (*list)->next; // skip '|'
		pipe->left = ft_build_tree(list);
		pipe->right = ft_build_tree(list);
		return ((void *)pipe);
	}
	else if (token->type == OUTFILE || token->type == INFILE || token->type == APPEND || token->type == HEREDOC)
	{
		redir = (t_redir *)malloc(sizeof(t_redir));
		if (!redir)
			return (NULL); //ft_error_hanlder(); malloc failed
		redir->token = token;
		if ((*list)->next && (*list)->next->content)
		{
			if (((t_token *)(*list)->next->content)->type == PIPE)
				printf("how to execute >|?\n");
			else
				redir->target = /* (t_token *) */(*list)->next->content;
		}
		redir->next = ft_build_tree(list);
		return ((void *)redir);
	}
	else if (token->type == EXEC)
	{
		exec = (t_exec *)malloc(sizeof(t_exec));
		if (!exec)
			return (NULL); //ft_error_hanlder(); malloc failed
		exec->token = token;
		exec->pathname = token->value;
		exec->args = ft_get_args(list);
		*list = (*list)->next;
		return ((void *)exec);
	}
	return (NULL);
}


// t_exec	*ft_init_exec(void)
// {
// 	t_exec	*exec;

// 	exec = (t_exec *)malloc(sizeof(t_exec));
// 	if (!exec)
// 		return (NULL);
// 	exec->token = NULL;
// 	exec->pathname = NULL;
// 	exec->args = NULL;
// 	return (exec);
// }

// void	*ft_parse_exec(t_list **list)
// {
// 	t_exec	*exec;
// 	t_token	*token;
// 	//t_list	*start;

// 	//token = (t_token *)(*list)->content;
// 	//if (ft_is_builtin(token))
// 	//	ft_exec_builtin(list);
// 	//else
// 	//start = *list;
// 	exec = ft_init_exec();
// 	if (!exec)
// 		return (NULL); // ft_error_handler(); malloc error
// 	while (*list)
// 	{
// 		token = (t_token *)(*list)->content;
// 		if (token->type == PIPE)
// 			break ;
// 		else if (token->type == APPEND || token->type == OUTFILE || token->type == HEREDOC || token->type == INFILE)
// 			*list = (*list)->next;
// 		else if (token->value[0] != '\0') 
// 		{
// 			if (!exec->token)
// 				exec->token = token;
// 			if (!exec->pathname)
// 				exec->pathname = token->value;
// 			exec->args = ft_add_to_vector(exec->args, token->value);
// 			if (!exec->args)
// 				return (free(exec), NULL); // ft_error_handler(); malloc error
// 		}
// 		*list = (*list)->next;
// 	}
// 	return (exec);
// }

// void	*ft_build_tree(t_list **list) //o que retornar? atualizar no header
// {
// 	t_list	*current;
// 	t_list	*start;
// 	t_token	*token;
// 	void	*root;

// 	root = NULL;
// 	if (!list || !*list)
// 	{
// 		printf("List is empty\n"); //delete later
// 		return (NULL);
// 	}
// 	start = *list;
// 	current = start;
// 	while (current)
// 	{
// 		token = (t_token *)current->content;
// 		//if (current == start && (token->type == EXPORT || token->type == EXPORT_AP))
// 		//	ft_export_to_local();
// 		//if (ft_search_pipe())
// 		//	ft_parse_pipe();
// 		//else if (ft_search_redir())
// 		//	ft_parse_redir();
// 		/* else */
// 		if (token->value[0] != '\0')
// 			root = ft_parse_exec(&current);
// 		current = current->next;
// 	}
// 	if (root)
// 		print_tree(root, 40); //delete later
// 	return (root);
// }
