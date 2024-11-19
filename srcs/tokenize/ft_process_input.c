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
	t_list	*current;
	t_token	*token;

	if (!root)
		return;
	// Check if the node is a PIPE
	if (((t_pipe *)root)->type == PIPE)
	{
		t_pipe *pipe_node = (t_pipe *)root;
		printf("%*sPIPE\n", indent, "");
		printf("%*s/\n", indent, "");
		if (pipe_node && pipe_node->left)
			print_tree(pipe_node->left, indent + 0);  // Print the left child with more indentation
		printf("%*s\\\n", indent + 10, "");
		if (pipe_node && pipe_node->right)
			print_tree(pipe_node->right, indent + 4); // Print the right child with more indentation
	}
	// Check if the node is a REDIRECTION
	else if (((t_redir *)root)->type == OUTFILE || ((t_redir *)root)->type == INFILE ||
			 ((t_redir *)root)->type == APPEND || ((t_redir *)root)->type == HEREDOC)
	{
		t_redir *redir_node = (t_redir *)root;
		printf("%*sREDIRECTION: %s ", indent - 5, "",
			   (redir_node->type == APPEND) ? "APPEND" :
			   (redir_node->type == HEREDOC) ? "HEREDOC" : //why is it printng heredoc when it's supposed to print outfile?
			   (redir_node->type == INFILE) ? "INFILE" :
			   "OUTFILE");

		// Safely check and print target value
		if (redir_node->target)
		{
			current = *(redir_node->target);
			while (current)
			{
				token = (t_token *)current->content;
				printf("%s\n", token->value);
				current = current->next;
			}
		}
		else
			printf("(no target)\n");

		if (redir_node && redir_node->next)
			print_tree(redir_node->next, indent + 4);  // Print the next node (could be another redir or exec)
	}
	// Check if the node is an EXEC
	else if (((t_exec *)root)->type == EXEC || ((t_exec *)root)->type == EXPORT || ((t_exec *)root)->type == EXPORT_AP)
	{
		t_exec *exec_node = (t_exec *)root;
		printf("%*sEXEC: %s\n", indent - 5, "", exec_node->pathname ? exec_node->pathname : "(no pathname)");
		if (exec_node && exec_node->args)
		{
			printf("%*sArguments:\n", indent - 5, "");
			current = *(exec_node->args);
			while (current)
			{
				token = (t_token *)current->content;
				printf("%*s%s\n", indent, "", token->value);
				current = current->next;
			}
		}
	}
	else
	{
		printf("%*sUNKNOWN NODE TYPE\n", indent, "");
	}
}


/**
 * @brief Processes the input string to build an execution tree.
 * 
 * This function validates the syntax of the input, tokenizes and processes the input
 * string, and builds an execution tree for commands, redirections, and pipes.
 * 
 * @param input The raw input string to process.
 * @param my_envp The environment variables for token expansion.
 * @return A pointer to the root of the execution tree if successful, or NULL on failure.
 */
void	*ft_process_input(char *input, char **my_envp)
{
	t_list	**token_list;
	char	*trimmed;
	void	*root;

	if (!ft_validate_syntax(input))
		return (NULL); //invalid syntax
	trimmed = ft_strtrim(input, ISSPACE);
	token_list = ft_create_token_list(trimmed);
	if (!token_list)
		return (NULL); //ft_error_handler // malloc failed
	ft_print_list(token_list); // debug
	//printf("after expansion:\n"); //debug
	//ft_process_token_list(token_list, my_envp); // will move to execution
	//ft_print_list(token_list); // debug
	
	if (token_list && *token_list)
	{
		root = ft_build_tree(token_list);
		if (root)
			print_tree(root, 40);
		ft_free_list(token_list); //update brief
		return (root);
	}
	ft_free_list(token_list);
	return (NULL);
	//ft_free_tree(root);
}
