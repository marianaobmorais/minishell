#include "../../includes/minishell.h"

char	*ft_expand_env(char *s, int *i, char **my_envp)
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

void	ft_handle_expansion(char **new_value, char *value, int *i, char **my_envp)
{
	char	*expansion;
	char	*tmp;

	(*i)++; // skip $
	if (value[(*i)] == '?')
		expansion = ft_get_exit_code(i);
	else
		expansion = ft_expand_env(&value[(*i)], i, my_envp);
	tmp = ft_strjoin(*new_value, expansion);
	free(*new_value);
	free(expansion);
	*new_value = tmp;
}

void	ft_handle_squotes(char **new_value, char *value, int *i)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[(*i)] && value[(*i)] != SQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
}

void	ft_handle_dquotes(char **new_value, char *value, int *i, char **my_envp)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[(*i)] && value[(*i)] != DQUOTE)
	{
		if (value[(*i)] == '$' && ((ft_isalpha(value[(*i) + 1]) || value[(*i) + 1] == '?' || value[(*i) + 1] == '_')))
			ft_handle_expansion(new_value, value, i, my_envp);
		else
			*new_value = ft_charjoin(*new_value, value[(*i)++]);
	}
	if (value[(*i)] == DQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
}

