#include "../../includes/minishell.h"

static void	ft_handle_quotes(char **new_value, char *value, int *i, char **my_envp)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[(*i)] && value[(*i)] != DQUOTE && value[(*i)] != SQUOTE)
	{
		if (value[(*i)] == '$' && ((ft_isalnum(value[(*i) + 1]) || value[(*i) + 1] == '?' || value[(*i) + 1] == '_')))
			ft_handle_expansion(new_value, value, i, my_envp);
		else
			*new_value = ft_charjoin(*new_value, value[(*i)++]);
	}
	if (value[(*i)] == DQUOTE || value[(*i)] == SQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
}

static char	*ft_expand_input(char *input, char **my_envp)
{
	char	*new_input;
	int		i;

	new_input = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == DQUOTE || input[i] == SQUOTE)
			ft_handle_quotes(&new_input, input, &i, my_envp);
		else if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '?' || input[i + 1] == '_'))
			ft_handle_expansion(&new_input, input, &i, my_envp);//
		else if (input[i] && input[i] != DQUOTE && input[i] != SQUOTE)
			new_input = ft_charjoin(new_input, input[i++]);
	}
	free(input);
	return (new_input);
}

static int	count_line(int mode)
{
	static int	line;

	if (line == 0)
		line = 1;
	if (mode == 1)
		line++;
	return (line);
}

static int	read_heredoc(char *limiter, int expand, char **my_envp)
{
	int		fd_write;
	char	*input;

	fd_write = open("/tmp/.heredoc_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	input = NULL;
	while (1)
	{
		if (input)
			free(input);
		input = readline("> ");
		if ((ft_strlen(input) != 0
			&& !ft_strncmp(limiter, input, ft_strlen(input))) || !input)
		{
			if (!input)
				ft_stderror("warning: here-document at line %d delimited by end-of-file (wanted `%s')", count_line(0), limiter);
			free(input);
			break ;
		}
		add_history(input);
		if (expand == FALSE)
			input = ft_expand_input(input, my_envp);
		ft_putendl_fd(input, fd_write);
		count_line(1);
	}
	close(fd_write);
	return (open("/tmp/.heredoc_tmp", O_RDONLY));
}

int	heredoc_fd(char *limiter, char **my_envp, int expand)
{
	int	fd;

	ft_signal(HEREDOC_); //corrigir
	fd = read_heredoc(limiter, expand, my_envp);
	dup2(fd, STDIN_FILENO);
	close(fd);
	unlink("/tmp/.heredoc_tmp");
	return (ft_exit_status(0, TRUE, FALSE));
}
