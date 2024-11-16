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
			if (current->next && ((t_token *)current->next->content)->type != PIPE) //the check current->next is redundant
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
		//printf("find_next_pipe loop: Current token: %s\n", token->value); // Debug
		if (token->type == PIPE)
		{
			*list = (*list)->next; // update list to token after pipe '|' before returning
			//printf("pipe: Found PIPE, updated list to: %s\n", (*list) ? ((t_token *)(*list)->content)->value : "NULL"); // debug
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
		//printf("find_next_redir loop: Current token: %s\n", token->value); // Debug
		if (token->type == PIPE)
		{
			//printf("find_next_redir loop: found pipe -> break\n"); //debug
			break ;
		}
		if (token->type == OUTFILE || token->type == INFILE || token->type == APPEND || token->type == HEREDOC)
		{
			//printf("redir: Found redir, updated list to: %s\n", (*list) ? ((t_token *)(*list)->content)->value : "NULL"); // debug
			return (true);
		}
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
		//printf("find_next_exec loop: Current token: %s\n", token->value); // Debug
		if (token->type == PIPE)
		{
			//printf("find_next_exec loop: found pipe -> break\n"); //debug
			break ;
		}
		if (token->type == EXEC)
		{
			//printf("exec: Found exec: %s\n", (*list) ? ((t_token *)(*list)->content)->value : "NULL"); // debug
			return (true);
		}
		*list = (*list)->next;
	}
	return (false);
}