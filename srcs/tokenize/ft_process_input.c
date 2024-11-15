#include "../../includes/minishell.h"

void	ft_print_list(t_list **token_list)
{
	t_list	*current;
	t_token	*token;

	if (!token_list || !*token_list)
	{
		printf("List is empty\n");
		return ;
	}
	printf("------------------------------------------------------\n");
	printf("| %-13s | %-8s | %-10s | %-10s |\n", "token", "type", "state", "expansion");
	printf("------------------------------------------------------\n");
	current = *token_list;
	while (current)
	{
		token = (t_token *)current->content;
		printf("| %-13s | %-8i | %-10i | %-10i |\n",
			token->value,
			token->type,
			token->state,
			token->expand);
		current = current->next;
	}
	printf("------------------------------------------------------\n");
}

void print_tree(void *root, int indent) // delete later
{
	if (!root)
		return;

	// Print the corresponding node type
	if (((t_pipe *)root)->type == PIPE)
	{
		t_pipe *pipe_node = (t_pipe *)root;
		printf("%*sPIPE\n", indent, "");
		printf("%*s/\n", indent, "");
		print_tree(pipe_node->left, indent + 4);  // Print the left child with more indentation
		printf("%*s\\\n", indent, "");
		print_tree(pipe_node->right, indent + 4); // Print the right child with more indentation
	}
	else if (((t_redir *)root)->type == OUTFILE || ((t_redir *)root)->type == INFILE ||
			 ((t_redir *)root)->type == APPEND || ((t_redir *)root)->type == HEREDOC)
	{
		t_redir *redir_node = (t_redir *)root;
		printf("%*sREDIRECTION: %s %s\n", indent, "",
			   (redir_node->type == APPEND) ? "APPEND" :
			   (redir_node->type == HEREDOC) ? "HEREDOC" :
			   (redir_node->type == INFILE) ? "INFILE" :
			   "OUTFILE", redir_node->target->value);
		if (redir_node->next)
			print_tree(redir_node->next, indent + 4);  // Print the next node (could be another redir or exec)
	}
	else if (((t_exec *)root)->type == EXEC)
	{
		t_exec *exec_node = (t_exec *)root;
		printf("%*sEXEC: %s\n", indent, "", exec_node->pathname);
		if (exec_node->args)
		{
			printf("%*sArguments:\n", indent + 4, "");
			for (int i = 0; exec_node->args[i] != NULL; i++)
			{
				printf("%*s%s\n", indent + 8, "", exec_node->args[i]);
			}
		}
	}
}

/**
 * @brief Parses and processes the input string, expanding variables and organizing tokens.
 * 
 * This function first validates the syntax of the input and removes leading/trailing whitespace.
 * It then tokenizes the input, expands any variables, removes quotes, and creates an binary tree
 * based on the tokens. Finally, it frees the allocated token list.
 * 
 * @param input The raw input string to process.
 * @param my_envp Environment variable array used for token expansion.
 */
void	ft_process_input(char *input, char **my_envp)
{
	t_list	**token_list;
	char	*trimmed;
	void	*root;

	if (!ft_validate_syntax(input))
		return ;
	trimmed = ft_strtrim(input, ISSPACE);
	token_list = ft_create_token_list(trimmed);
	if (!token_list)
		return ;
	ft_print_list(token_list); // delete later
	printf("after expansion:\n");
	ft_process_token_list(token_list, my_envp);
	ft_print_list(token_list); // delete later
	//create tree
	if (token_list && *token_list)
	{
		root = ft_build_tree(token_list);
		if (root)
			print_tree(root, 40);
		//if (!root)
		//	??
	}
	ft_free_list(token_list);
}
