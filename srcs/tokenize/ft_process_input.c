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

void print_root(void *root, int indent) // Updated function
{
	t_list  *current;
	t_token *token;

	if (!root)
	{
		printf("!root\n");
		return ;
	}

	// Check if the node is ROOT
	if (((t_node *)root)->type == ROOT)
	{
		t_node *root_node = (t_node *)root;
		printf("%*sROOT\n", indent + 10, "");
		printf("%*s/\n", indent + 10, "");
		if (root_node->left)
			print_root(root_node->left, indent + 2); // Print left child
		printf("%*s\\\n", indent + 40, "");
		if (root_node->right)
			print_root(root_node->right, indent + 40); // Print right child
	}
	// Check if the node is AND
	else if (((t_node *)root)->type == AND)
	{
		t_node *and_node = (t_node *)root;
		printf("%*sAND\n", indent, "");
		printf("%*s/\n", indent, "");
		if (and_node->left)
			print_root(and_node->left, indent - 15); // Print left child
		printf("%*s\\\n", indent + 20, "");
		if (and_node->right)
			print_root(and_node->right, indent + 10); // Print right child
	}
	// Check if the node is OR
	else if (((t_node *)root)->type == OR)
	{
		t_node *or_node = (t_node *)root;
		printf("%*sOR\n", indent, "");
		printf("%*s/\n", indent, "");
		if (or_node->left)
			print_root(or_node->left, indent - 15); // Print left child
		printf("%*s\\\n", indent + 20, "");
		if (or_node->right)
			print_root(or_node->right, indent + 10); // Print right child
	}
	// Check if the node is PIPE
	else if (((t_node *)root)->type == PIPE)
	{
		t_node *pipe_node = (t_node *)root;
		t_node	*parent = (t_node *)pipe_node->parent_node;
		printf("%*sPIPE\n", indent - 7, "");
		printf("%*sparent: %s\n", indent - 7,
				"", 
				(parent->type == ROOT) ? "ROOT" :
				(parent->type == SUB_ROOT) ? "SUB_ROOT" :
				(parent->type == AND) ? "AND" : "OR");
		printf("%*s//\n", indent - 7, "");
		if (pipe_node->left)
			print_root(pipe_node->left, indent + 0); // Print left child
		printf("%*s\\\\\n", indent, "");
		if (pipe_node->right)
			print_root(pipe_node->right, indent + 5); // Print right child
	}
	// Check if the node is a REDIRECTION
	else if (((t_redir *)root)->type == OUTFILE || ((t_redir *)root)->type == INFILE ||
			 ((t_redir *)root)->type == APPEND || ((t_redir *)root)->type == HEREDOC)
	{
		t_redir *redir_node = (t_redir *)root;
		printf("%*sREDIR: %s ", indent - 10,
			   "",
			   (redir_node->type == APPEND) ? "APPEND" :
			   (redir_node->type == HEREDOC) ? "HEREDOC" :
			   (redir_node->type == INFILE) ? "INFILE" : "OUTFILE");
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
		if (redir_node->next)
			print_root(redir_node->next, indent + 2); // Print next node
	}
	// Check if the node is EXEC
	else if (((t_exec *)root)->type == EXEC || ((t_exec *)root)->type == EXPORT ||
			 ((t_exec *)root)->type == EXPORT_AP)
	{
		t_exec *exec_node = (t_exec *)root;
		printf("%*sEXEC: %s\n", indent - 10, "", exec_node->pathname ? exec_node->pathname : "(no pathname)");
		if (exec_node->args)
		{
			printf("%*sArguments:\n", indent - 10, "");
			current = *(exec_node->args);
			while (current)
			{
				token = (t_token *)current->content;
				printf("%*s %s\n", indent - 8, "", token->value);
				current = current->next;
			}
		}
	}
	else if (((t_node *)root)->type == SUB_ROOT)
	{
		t_node *subroot_node = (t_node *)root;
		printf("%*sSUB_ROOT\n", indent - 20, "");
		printf("%*s///\n", indent - 20, "");
		if (subroot_node->left)
			print_root(subroot_node->left, indent - 20);
		printf("%*s\\\\\\\n", indent - 10, "");
		if (subroot_node->right)
			print_root(subroot_node->right, indent - 10);
	}
	else
		printf("Type not found\n");
}

/**
 * @brief Processes the input string to build an execution tree.
 * 
 * This function takes input string, trims leading and trailing whitespace,
 * validates its syntax, and tokenizes it into a list. It then constructs a
 * syntax tree representing the input commands and redirections. If any step
 * fails, the function cleans up allocated memory and returns NULL.
 * 
 * @param input The raw input string to process.
 * @return A pointer to the root of the syntax tree (AST) if successful, or
 *         NULL on failure.
 */
void	*ft_process_input(char *input)
{
	t_list	**token_list;
	char	*trimmed;
	void	*root;

	trimmed = ft_strtrim(input, ISSPACE); 
	if (!trimmed)
		return (NULL); //ft_error_handler; 1 // malloc failed 
	if (!ft_validate_syntax(trimmed))
		return (free(trimmed), NULL);
	token_list = ft_create_token_list(trimmed);
	if (!token_list)
		return (free(trimmed), NULL);
	ft_print_list(token_list); // debug
	root = NULL;
	if (token_list && *token_list)
	{
		root = ft_build_root(token_list, ROOT);
		print_root(root, 60); //debug
	}
	free(trimmed);
	ft_free_list(token_list);
	return (root);
}
