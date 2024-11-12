#include "../../includes/minishell.h"

void print_tree(void *root, int left, int right)
{
	int type; //enum?
	int spacing;

	if (!root)
		return;
	type = *((int *)root);
	spacing = 5;
	if (type == EXEC)
	{
		t_exec *cmd_node = (t_exec *)root;
		printf("%*sCMD: ", left, "");
		for (int i = 0; cmd_node->args && cmd_node->args[i]; i++) {
			printf("%s ", cmd_node->args[i]);
		}
		printf("\n");
	}
	else if (type == OUTFILE || type == INFILE || type == APPEND || type == HEREDOC) {
		t_redir *redir_node = (t_redir *)root;
		printf("%*s%s %s\n", left, "", (redir_node->token->type == APPEND)? "APPEND" :
										(redir_node->token->type == HEREDOC)? "HEREDOC":
										(redir_node->token->type == INFILE)? "INFILE":
										 "OUTFILE" , redir_node->next_token->value);
		printf("%*s|\n", left + spacing, "");
		print_tree(redir_node->next, left, right);
	}
	else if (type == PIPE) {
		t_pipe *pipe_node = (t_pipe *)root;
		printf("%*sPIPE\n", left, "");
		printf("%*s/\n", left, "");
		print_tree(pipe_node->left, left - spacing, right);
		printf("%*s\\\n", left + spacing, "");
		print_tree(pipe_node->right, left + spacing, right);
	}
}

void	*ft_parse_exec(t_list **list)
{
	t_exec	*exec;
	t_token	*token;
	//t_list	*start;

	exec = (t_exec *)malloc(sizeof(t_exec *));
	if (!exec)
		return (NULL);
	//start = *list;
	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->value[0] != '\0')
		{
			if (!exec->token)
				exec->token = token;
			if (!exec->pathname)
				exec->pathname = token->value;
			exec->args = ft_split(token->value, 32); //ft_add_to_vector(token->value);
		}
		*list = (*list)->next;
	}
	return (exec);
}

void	*ft_create_tree(t_list **list) //o que retornar? atualizar no header
{
	t_list	*current;
	t_token	*token;
	void	*root;

	root = NULL;
	if (!list || !*list)
	{
		printf("List is empty\n");//delete later
		return (NULL);
	}
	current = *list;
	while (current)
	{
		token = (t_token *)current->content;
		//if (ft_search_pipe())
		//	ft_parse_pipe();
		//else if (ft_search_redir())
		//	ft_parse_redir();
		/* else */
		if (token->value[0] != '\0')
			root = ft_parse_exec(&current);
		current = current->next;
	}
	print_tree(root, 40, 40);
	return (root);
}