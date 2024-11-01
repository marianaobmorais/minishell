#include "../../includes/minishell.h"

t_type	ft_get_node_type(char *str)
{
	//t_type	type;

	if (str[0] == '|')
		return (PIPE);
	else
		return (BUILTIN);
}

t_list	**ft_create_token_list(char *s)
{
	t_list	**token_list;
	t_list	*new_node;
	bool	in_token;
	char	*value;
	int		i;

	token_list = (t_list **)malloc(sizeof(t_list **));
	in_token = false;
	i = 0;
	value = NULL;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
		{
			if (ft_strchr(METACHARS, s[i]))
			{
				if (in_token)
				{
					new_node = ft_lstnew((t_token *)value); // need to study this more
					//new_node->type = ??
					ft_lstadd_back(token_list, new_node);
					free(value);
					in_token = false;
				}
				value = ft_charjoin(value, s[i]); // start new token for meta char
				if (ft_strchr(METACHARS, s[i + 1])) // if it's >> or <<
					value = ft_charjoin(value, s[i]);
				new_node = ft_lstnew((t_token *)value); // need to study this more
				ft_lstadd_back(token_list, new_node);
				free(value);
				in_token = false;
			}
			else if (s[i] == 39) // single quote
			{
				value = ft_charjoin(value, s[i]);
				while (s[i] != 39)
				{
					value = ft_charjoin(value, s[i]);
					i++;
				}
				value = ft_charjoin(value, s[i]);
				in_token = true;
			}
			else if (s[i] == 34) // double quotes
			{
				value = ft_charjoin(value, s[i]);
				while (s[i] != 34)
				{
					value = ft_charjoin(value, s[i]);
					i++;
				}
				value = ft_charjoin(value, s[i]);
				in_token = true;
			}
			else
			{
				value = ft_charjoin(value, s[i]);
				in_token = true;
			}
		}
		if (ft_isspace(s[i]) && in_token)
		{
			new_node = ft_lstnew((t_token *)value); // need to study this more
			//new_node->type = ??
			ft_lstadd_back(token_list, new_node);
			free(value);
			in_token = false;
		}
		i++;
	}
	free(s);
	//if (value)
	//	free(value);
	return (token_list);
}

void	ft_process_input(char *input, char **my_envp)
{
	(void)my_envp;
	t_list	**token_list;
	char	*trimmed;

	if (!ft_check_syntax(input))
		return ;
	trimmed = ft_strtrim(input, ISSPACE);
	token_list = ft_create_token_list(trimmed);
}
