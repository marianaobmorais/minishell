#include "../../includes/minishell.h"

char	*ft_expand(char *s, int *i, char **my_envp)
{
	char	*expansion;
	char	*env;
	char	*env_equal;
	int		len;
	int		j;

	len = 0;
	while (ft_isalpha(s[len]) || ft_isdigit(s[len]) || s[len] == '_')
		len++;
	env = ft_substr(s, 0, len);
	env_equal = ft_strjoin(env, "=");
	j = 0;
	while (my_envp[j])
	{
		if (ft_strncmp(env_equal, my_envp[j], (len + 1)) == 0)
		{
			expansion = ft_strdup(my_envp[j] + (len + 1));
			(*i) += len; // update index past env name
			return (free(env_equal), expansion);
		}
		j++;
	}
	(*i) += len; // update index past env name
	return (free(env_equal), ft_strdup("\0"));
}

char	*ft_get_exit_code(int *i)
{
	(*i)++;
	return (ft_strdup("EXIT_CODE")); // to do
}

/* void	ft_handle_expansion(t_token **token, char **my_envp)
{
	char	*expansion;
	char	*new_value;
	char	*tmp;
	int		i;

	new_value = NULL;
	i = 0;
	while ((*token)->value[i])
	{
		if ((*token)->value[i] == '$' && ((ft_isalpha((*token)->value[i + 1]) || (*token)->value[i + 1] == '?' || (*token)->value[i + 1] == '_')))
		{
			i++;
			if ((*token)->value[i] == '?')
			{
				expansion = ft_get_exit_code(&i);
				tmp = ft_strjoin(new_value, expansion);
				free(new_value);
				free(expansion);
				new_value = tmp;
			}
			else
			{
				expansion = ft_expand(&(*token)->value[i], &i, my_envp);
				tmp = ft_strjoin(new_value, expansion);
				free(new_value);
				free(expansion);
				new_value = tmp;
			}
		}
		else if ((*token)->value[i])
		{
			if ((*token)->value[i] == SQUOTE)
			{
				new_value = ft_charjoin(new_value, (*token)->value[i++]);
				while ((*token)->value[i] != SQUOTE)
					new_value = ft_charjoin(new_value, (*token)->value[i++]);
			}
			if ((*token)->value[i])
				new_value = ft_charjoin(new_value, (*token)->value[i++]);
		}
	}
	free((*token)->value);
	(*token)->value = new_value;
} */

// void	ft_handle_expansion(char **new_value, char *s, int *i, char **my_envp)
// {
// 	char	*expansion;
// 	char	*tmp;
// 	(*i)++;
// 	if (s[(*i)] == '?')
// 	{
// 		expansion = ft_get_exit_code(i);
// 		tmp = ft_strjoin(*new_value, expansion);
// 		free(*new_value);
// 		free(expansion);
// 		new_value = &tmp;
// 	}
// 	else
// 	{
// 		expansion = ft_expand(&s[(*i)], i, my_envp);
// 		tmp = ft_strjoin(*new_value, expansion);
// 		free(*new_value);
// 		free(expansion);
// 		new_value = &tmp;
// 	}
// }

void	ft_parse_quote(t_token *token, t_state state)
{
	(void)token;
	(void)state;
	return ;
}

void	ft_parse_token_list(t_list **list, char **my_envp)
{
	t_list	*current;
	t_token	*token;

	current = *list;
	while (current)
	{
		token = (t_token *)current->content;
		if (token->expand == true)
		{
			//ft_handle_expansion(&token, my_envp);
			//
			char	*expansion;
			char	*new_value;
			char	*tmp;
			int		i;

			new_value = NULL;
			i = 0;
			while (token->value[i])
			{
				if (token->value[i] == SQUOTE)
				{
					new_value = ft_charjoin(new_value, token->value[i++]);
					while (token->value[i] && token->value[i] != SQUOTE)
						new_value = ft_charjoin(new_value, token->value[i++]);
					new_value = ft_charjoin(new_value, token->value[i]);
				}
				else if (token->value[i] == DQUOTE)
				{
					new_value = ft_charjoin(new_value, token->value[i++]);
					while (token->value[i] && token->value[i] != DQUOTE)
					{
						if (token->value[i] == '$' && ((ft_isalpha(token->value[i + 1]) || token->value[i + 1] == '?' || token->value[i + 1] == '_')))
						{
							//ft_handle_expansion(&new_value, &token->value[i], &i, my_envp);
							i++;
							if (token->value[i] == '?')
							{
								expansion = ft_get_exit_code(&i);
								tmp = ft_strjoin(new_value, expansion);
								free(new_value);
								free(expansion);
								new_value = tmp;
							}
							else
							{
								expansion = ft_expand(&token->value[i], &i, my_envp);
								tmp = ft_strjoin(new_value, expansion);
								free(new_value);
								free(expansion);
								new_value = tmp;
							}
						}
						else
							new_value = ft_charjoin(new_value, token->value[i++]);
					}
				}
				else if (token->value[i] == '$' && ((ft_isalpha(token->value[i + 1]) || token->value[i + 1] == '?' || token->value[i + 1] == '_')))
				{
					//ft_handle_expansion(&new_value, &token->value[i], &i, my_envp);
					i++;
					if (token->value[i] == '?')
					{
						expansion = ft_get_exit_code(&i);
						tmp = ft_strjoin(new_value, expansion);
						free(new_value);
						free(expansion);
						new_value = tmp;
					}
					else
					{
						expansion = ft_expand(&token->value[i], &i, my_envp);
						tmp = ft_strjoin(new_value, expansion);
						free(new_value);
						free(expansion);
						new_value = tmp;
					}
				}
				else if (token->value[i] && (token->value[i] != DQUOTE))
					new_value = ft_charjoin(new_value, token->value[i++]);
			}
			free(token->value);
			token->value = new_value;
			//
		}
		current = current->next;
	}
}
