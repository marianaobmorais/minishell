#include "../../includes/minishell.h"

void	ft_handle_quotes_hd(char **new_value, char *value, int *i, char **envp)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[(*i)] && value[(*i)] != DQUOTE && value[(*i)] != SQUOTE)
	{
		if (value[(*i)] == '$'
			&& ((ft_isalnum(value[(*i) + 1])
					|| value[(*i) + 1] == '?' || value[(*i) + 1] == '_')))
			ft_handle_expansion(new_value, value, i, envp);
		else
			*new_value = ft_charjoin(*new_value, value[(*i)++]);
	}
	if (value[(*i)] == DQUOTE || value[(*i)] == SQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
}

char	*ft_expand_input(char *input, char **envp)
{
	char	*new_input;
	int		i;

	new_input = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == DQUOTE || input[i] == SQUOTE)
			ft_handle_quotes_hd(&new_input, input, &i, envp);
		else if (input[i] == '$' && (ft_isalnum(input[i + 1])
				|| input[i + 1] == '?' || input[i + 1] == '_'))
			ft_handle_expansion(&new_input, input, &i, envp);
		else if (input[i] && input[i] != DQUOTE && input[i] != SQUOTE)
			new_input = ft_charjoin(new_input, input[i++]);
	}
	free(input);
	return (new_input);
}

int	count_line(int mode)
{
	static int	line;

	if (line == 0)
		line = 1;
	if (mode == 1)
		line++;
	return (line);
}

char	*ft_create_pathname(void)
{
	char		*base;
	char		*pathname;
	char		*temp;
	static int	i;

	base = "/tmp/.heredoc_";
	temp = ft_itoa(i);
	pathname = ft_strjoin(base, temp);
	free(temp);
	unlink(pathname);
	i++;
	return (pathname);
}
