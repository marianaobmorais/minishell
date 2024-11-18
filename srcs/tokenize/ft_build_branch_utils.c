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
	t_list	*curr;
	t_token	*token;

	args = NULL;
	curr = *list;
	while (curr)
	{
		token = (t_token *)curr->content;
		if (token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP)
			args = ft_add_to_vector(args, token->value);
		else if (token->type == APPEND || token->type == OUTFILE
				|| token->type == HEREDOC || token->type == INFILE)
		{
			if (curr->next && ((t_token *)curr->next->content)->type != PIPE) //the check curr->next is redundant
				curr = curr->next;
		}
		else if (token->type == PIPE)
			break ;
		curr = curr->next;
	}
	return (args);
}

bool	ft_find_next_redir(t_list **list)
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
			break ;
		if (token->type == OUTFILE || token->type == INFILE
				|| token->type == APPEND || token->type == HEREDOC)
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
