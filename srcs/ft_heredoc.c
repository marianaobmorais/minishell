#include "../includes/minishell.h"

int	count_line(int mode)
{
	static int	line;

	if (line == 0)
		line = 1;
	if (mode == 1)
		line++;
	return (line);
}

static int	read_heredoc(char *limiter)
{
	int		fd_write;
	char	*input;

	fd_write = open("/tmp/.heredoc_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	input = NULL;
	ft_signal(HEREDOC); //corrigir
	while (1)
	{
		if (input)
		{
			free(input);
			input = NULL;
		}
		input = readline("> ");
		if (!input)
		{
			free(input);
			ft_stderror("warning: here-document at line %d delimited by end-of-file (wanted `%s')", count_line(0), limiter);
			break ;
		}
		if (ft_strlen(input) != 0 && !ft_strncmp(limiter, input, ft_strlen(input)))
		{
			free(input);
			break ;
		}
		//verificar se realmente precisa salvar o historico
		ft_putendl_fd(input, fd_write);
		count_line(1);
	}
	close(fd_write);
	return (open("/tmp/.heredoc_tmp", O_RDONLY));
}

int	heredoc_fd(char *limiter)
{
	int	fd;

	fd = read_heredoc(limiter);
	//ft_signal(DEFAULT);
	dup2(fd, STDIN_FILENO);
	//para enviar para outro processo precisa de pipe ?
	close(fd);
	unlink("/tmp/.heredoc_tmp");
	return (ft_exit_status(0, TRUE, FALSE));
}
