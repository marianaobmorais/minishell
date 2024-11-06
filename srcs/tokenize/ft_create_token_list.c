#include "../../includes/minishell.h"

static int	ft_handle_metachar(char **value, char *s, int i, t_list **token_list)
{
	*value = ft_charjoin(*value, s[0]);
	if (ft_strchr(METACHARS, s[1])) // if it's >> or <<
	{
		*value = ft_charjoin(*value, s[1]);
		i++;
	}
	ft_add_to_token_list(value, token_list);
	return (i);
}

static int	ft_handle_quotes(char **value, char *s, int i, char quote)
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

static void	ft_process_tokens(char *s, t_list **token_list)
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
			{
				if (value)
					ft_add_to_token_list(&value, token_list);
				i = ft_handle_metachar(&value, &s[i], i, token_list);
			}
			else if (s[i] == SQUOTE || s[i] == DQUOTE)
				i = ft_handle_quotes(&value, s, i, s[i]);
			else
				value = ft_charjoin(value, s[i]);
		}
		if (ft_isspace(s[i]) && value)
			ft_add_to_token_list(&value, token_list);
		i++;
	}
	ft_add_to_token_list(&value, token_list);
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
