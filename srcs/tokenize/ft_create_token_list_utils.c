#include "../../includes/minishell.h"

static t_type	ft_get_cmd_type(char *s)
{
	int	i;

	i = 0;
	if (ft_isalpha(s[i]) || s[i] == '_')
	{
		while (ft_isalnum(s[i]) || s[i] == '_')
			i++;
		if (s[i] == '+' && s[i + 1] == '=')
			return (EXPORT_AP);
		else if (s[i] == '=')
			return (EXPORT);
	}
	return (EXEC);
}

static t_type	ft_get_token_type(char *s)
{
	t_type	type;

	if (s[0] == '|')
		return (PIPE);
	else if (s[0] == '>')
	{
		if (s[1] == '>')
			return (APPEND);
		return (OUTFILE);
	}
	else if (s[0] == '<')
	{
		if (s[1] == '<')
			return (HEREDOC);
		return (INFILE);
	}
	else
	{
		type = ft_get_cmd_type(s);
		return (type);
	}
}

static t_state	ft_get_token_state(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == SQUOTE || s[i] == DQUOTE)
			return (IN_QUOTE);
		i++;
	}
	return (GENERAL);
}

static bool	ft_has_expandable_var(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == SQUOTE)
			i = ft_next_quote(&s[i], i, SQUOTE) + 1;
		else if (s[i] == DQUOTE)
		{
			i++;
			while (s[i] && s[i] != DQUOTE)
			{
				if (s[i] == '$' && (ft_isalpha(s[i + 1]) || s[i + 1] == '?' || s[i + 1] == '_'))
					return (true);
				i++;
			}
		}
		else if (s[i] == '$' && (ft_isalpha(s[i + 1]) || s[i + 1] == '?' || s[i + 1] == '_'))
			return (true);
		else if (s[i])
			i++;
	}
	return (false);
}

void	ft_add_to_token_list(char **value, t_list **token_list)
{
	t_token	*new_token;
	t_list	*new_node;

	if (*value)
	{
		new_token = (t_token *)malloc(sizeof(t_token));
		if (!new_token)
			return ; // ft_error_handler();
		new_token->value = ft_strdup(*value);
		new_token->type = ft_get_token_type(*value);
		new_token->state = ft_get_token_state(*value);
		new_token->expand = ft_has_expandable_var(*value);
		new_node = ft_lstnew((t_token *)new_token);
		if (!new_node)
			return ;
		ft_lstadd_back(token_list, new_node);
		free(*value);
		*value = NULL;
	}
}
