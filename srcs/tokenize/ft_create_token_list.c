#include "../../includes/minishell.h"

t_type	ft_get_type(char *s)
{
	//int		i;
	//bool	eligible_export;
	//bool	export;

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
		//export
		//eligible_export = true;
		//export = false;
		//i = 0;
		//while (s[i])
		//{
		//	if ()
		//	i++;
		//}
		//if (export)
		//	return (EXPORT);
		return (EXEC);
	}
}

t_state	ft_get_state(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == SQUOTE)
			return (IN_SQUOTE);
		else if (s[i] == DQUOTE)
			return (IN_DQUOTE);
		i++;
	}
	return (GENERAL);
}

bool	ft_is_expansion(char *s, t_state state)
{
	int	i;

	if (state == GENERAL || state == IN_DQUOTE)
	{
		i = 0;
		while (s[i])
		{
			if (s[i] == '$' && (ft_isalpha(s[i + 1]) || s[i + 1] == '?' || s[i + 1] == '_'))
				return (true);
			i++;
		}
	}
	return (false);
}

void	ft_add_to_list(char **value, t_list **token_list)
{
	t_token	*new_token;
	t_list	*new_node;

	if (*value)
	{
		new_token = (t_token *)malloc(sizeof(t_token));
		if (!new_token)
			return ; // ft_error_handler();
		new_token->value = ft_strdup(*value);
		new_token->type = ft_get_type(*value);
		new_token->state = ft_get_state(*value);
		new_token->expand = ft_is_expansion(*value, new_token->state);
		new_node = ft_lstnew((t_token *)new_token);
		if (!new_node)
			return ;
		ft_lstadd_back(token_list, new_node);
		free(*value);
		*value = NULL;
	}
}

int	ft_handle_metachar(char **value, char *s, int i, t_list **token_list)
{
	if (*value)
		ft_add_to_list(value, token_list);
	*value = ft_charjoin(*value, s[0]);
	if (ft_strchr(METACHARS, s[1])) // if it's >> or <<
	{
		*value = ft_charjoin(*value, s[1]);
		i++;
	}
	ft_add_to_list(value, token_list);
	return (i);
}

int	ft_handle_quotes(char **value, char *s, int i, char quote)
{
	*value = ft_charjoin(*value, s[i]);
	i++;
	while (s[i] != quote)
	{
		*value = ft_charjoin(*value, s[i]);
		i++;
	}
	*value = ft_charjoin(*value, s[i]);
	return (i);
}

void	ft_process_tokens(char *s, t_list **token_list)
{
	char	*value;
	int		i;

	i = 0;
	value = NULL;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
		{
			if (ft_strchr(METACHARS, s[i]))
				i = ft_handle_metachar(&value, &s[i], i, token_list);
			else if (s[i] == SQUOTE || s[i] == DQUOTE)
				i = ft_handle_quotes(&value, s, i, s[i]);
			else
				value = ft_charjoin(value, s[i]);
		}
		if (ft_isspace(s[i]) && value)
			ft_add_to_list(&value, token_list);
		i++;
	}
	ft_add_to_list(&value, token_list);
}

t_list	**ft_create_token_list(char *s)
{
	t_list	**token_list;

	token_list = (t_list **)malloc(sizeof(t_list **));
	if (!token_list)
		return (NULL); // ft_error_handler();
	*token_list = NULL;
	ft_process_tokens(s, token_list);
	return (free(s), token_list);
}
