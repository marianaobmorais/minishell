#include "../../includes/minishell.h"

void print_tree(void *root, int indent) // delete later
{
	if (!root)
		return;

	// Print the corresponding node type
	if (((t_pipe *)root)->token->type == PIPE)
	{
		t_pipe *pipe_node = (t_pipe *)root;
		printf("%*sPIPE\n", indent, "");
		printf("%*s/\n", indent, "");
		print_tree(pipe_node->left, indent + 4);  // Print the left child with more indentation
		printf("%*s\\\n", indent, "");
		print_tree(pipe_node->right, indent + 4); // Print the right child with more indentation
	}
	else if (((t_redir *)root)->token->type == OUTFILE || ((t_redir *)root)->token->type == INFILE ||
			 ((t_redir *)root)->token->type == APPEND || ((t_redir *)root)->token->type == HEREDOC)
	{
		t_redir *redir_node = (t_redir *)root;
		printf("%*sREDIRECTION: %s %s\n", indent, "",
			   (redir_node->token->type == APPEND) ? "APPEND" :
			   (redir_node->token->type == HEREDOC) ? "HEREDOC" :
			   (redir_node->token->type == INFILE) ? "INFILE" :
			   "OUTFILE", redir_node->next_token->value);
		if (redir_node->next)
			print_tree(redir_node->next, indent + 4);  // Print the next node (could be another redir or exec)
	}
	else if (((t_exec *)root)->token->type == EXEC)
	{
		t_exec *exec_node = (t_exec *)root;
		printf("%*sEXEC: %s\n", indent, "", exec_node->pathname);
		if (exec_node->args)
		{
			printf("%*sArguments:\n", indent + 4);
			for (int i = 0; exec_node->args[i] != NULL; i++)
			{
				printf("%*s%s\n", indent + 8, "", exec_node->args[i]);
			}
		}
	}
}


// char	**ft_add_to_vector(char **vector, char *new_str)
// {
// 	char	**res;
// 	int		i;

// 	i = 0;
// 	while (vector[i])
// 		i++;
// 	res = (char **)malloc(sizeof(char **) * (i + 2)); // + 2: one for new string and one for NULL
// 	if (!res)
// 		return (NULL); 
// 	i = 0;
// 	while (vector[i])
// 	{
// 		res[i] = ft_strdup(vector[i]);
// 		i++;
// 	}
// 	res[i++] = ft_strdup(new_str);
// 	res[i] = NULL;
// 	if (vector)
// 		ft_free_vector(vector);
// 	return (res);
// }

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

void	*ft_build_tree(t_list **list) //o que retornar? atualizar no header
{
	t_list	*current;
	t_list	*start;
	t_token	*token;
	void	*root;

	root = NULL;
	if (!list || !*list)
	{
		printf("List is empty\n");//delete later
		return (NULL);
	}
	start = *list;
	current = start;
	while (current)
	{
		token = (t_token *)current->content;
		//if (current == start && (token->type == EXPORT || token->type == EXPORT_AP))
		//	ft_export_to_local();
		//if (ft_search_pipe())
		//	ft_parse_pipe();
		//else if (ft_search_redir())
		//	ft_parse_redir();
		/* else */
		if (token->value[0] != '\0')
			root = ft_parse_exec(&current);
		current = current->next;
	}
	if (root)
		print_tree(root, 40); //delete later
	return (root);
}
