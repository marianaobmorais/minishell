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

static int	count_line(int mode) //nao funcionando dentro do child
{
	static int	line;

	if (line == 0)
		line = 1;
	if (mode == 1)
		line++;
	return (line);
}

static void	read_heredoc(char *eof, int state, char **my_envp, int fd_write)
{
	char	*input;

	input = NULL;
	ft_signal(HEREDOC_);
	while (1)
	{
		if (input)
			free(input);
		input = readline("> ");
		if (!input || !ft_strcmp(eof, input))
		{
			if (!input)
				ft_stderror(FALSE, "warning: here-document at line %d"\
				" delimited by end-of-file (wanted `%s')", count_line(0), eof);
			free(input);
			break ;
		}
		add_history(input);
		if (state == GENERAL)
			input = ft_expand_input(input, my_envp);
		ft_putendl_fd(input, fd_write);
		count_line(1);
	}
}

int	heredoc_fd(char *eof, char **my_envp, int state)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), -1);
	pid = fork();
	if (pid == -1)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), -1);
	if (pid == 0)
	{
		close(fd[0]);
		read_heredoc(eof, state, my_envp, fd[1]);
		close(fd[1]);
		ft_exit_status(0, TRUE, TRUE);
	}
	waitpid(pid, NULL, 0);
	close(fd[1]);
	// dup2(fd[0], STDIN_FILENO);
	// close(fd[0]);
	ft_exit_status(0, TRUE, FALSE);
	return (fd[0]);
}
