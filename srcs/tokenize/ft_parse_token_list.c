#include "../../includes/minishell.h"

/* char	*ft_expand(char *s, char **my_envp)
{
	char	*word;
	char	*env;
	char	*env_equal;
	int		len;
	int		i;

	i = 0;
	if (s[i] == '?') // to do
	{
		printf("exit code goes here\n");
	}
	while (ft_isalpha(s[i]))
		i++;
	env = ft_substr(s, 0, i);
	env_equal = ft_strjoin(env, "=");
	len = ft_strlen(env_equal);
	//printf("env = %s, len = %d\n", env_equal, len);//
	i = 0;
	while (my_envp[i])
	{
		if (ft_strncmp(env_equal, my_envp[i], len) == 0)
		{
			word = ft_strdup(my_envp[i] + len);
			return (free(env_equal), word);
		}
		i++;
	}
	return (free(env_equal), ft_strdup("\0"));
} */

/* if (s[i] == '$')
{
	i++;
	expansion = ft_expand_env(&s[i], my_envp);
	tmp = ft_strjoin(word, expansion);
	free(word);
	word = tmp;
	while (ft_isalpha(s[i]))
		i++;
} */

char	*ft_expand(char *s, char **envp)
{
	char	new_value;
	int		i;

	new_value = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
		{
			i++;
			if (s[i] == '?') // to do
				printf("exit code goes here\n");
			else
				while ()
		}
		new_value = ft_charjoin(new_value, s[i]);
		i++;
	}
	free(s);
	return (new_value);
}

void	ft_parse_quote(t_token *token, t_state state)
{
	return ;
}

void	ft_parse_token_list(t_list **list, char **my_envp)
{
	t_list	*current;
	t_token	*token;
	char	*expansion;
	char	*new_value;
	int		i;

	new_value = NULL;
	current = list;
	while (current)
	{
		if (token->state == EXPAND || token->state == DQUOTE)
		{
			while (token->value[i])
			{
				if (token->value[i] == '$' && (ft_isalpha(token->value[i + 1]) || token->value[i + 1] == '?'))
				{
					new_value = ft_expand(token->value, my_envp);
					//free(token->value);
					//token->value = new_value;
				}
				new_value = ft_charjoin()
				i++;
			}
		}
		//else
		//	ft_parse_quote(token, token->state);

		current = current->next;
	}
}