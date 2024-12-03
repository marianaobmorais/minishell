#include "../../includes/minishell.h"

bool	ft_is_heredoc_target(t_list **list)
{
	t_list	*last;
	t_token	*token;

	last = ft_lstlast(*list);
	if (!last || !last->content)
		return (false);
	token = (t_token *)last->content;
	if (token->type == HEREDOC)
		return (true);
	return (false);
}

bool	ft_is_wildcard(char *s)
{
	//write brief
	int		i;
	bool	wildcard;

	i = 0;
	wildcard = false;
	while (s[i])
	{
		if (s[i] == SQUOTE)
		{
			i++;
			while (s[i] && s[i] != SQUOTE)
			{
				if (s[i] == '*') 
					return (false);
				i++;
			}
			i++;
		}
		else if (s[i] == DQUOTE)
		{
			i++;
			while (s[i] && s[i] != DQUOTE)
			{
				if (s[i] == '*' && s[i - 1] == '$')
					wildcard = false;
				else if (s[i] == '*' && s[i - 1] != '$') 
					return (false);
				i++;
			}
			i++;
		}
		else if (s[i] == '*' && s[i - 1] != '$')
		{
			wildcard = true;
			i++;
		}
		else if (s[i] == '/')
			return (false);
		else if (s[i])
			i++;
	}
	return (wildcard);
}
