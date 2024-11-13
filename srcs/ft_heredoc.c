#include "../includes/minishell.h"

static int	read_heredoc(char *limiter)
{
	int		fd_write;
	//int		fd_read;
	char	*input;
	static int	line;

	//fd_write = open("/tmp/.heredoc_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	fd_write = open("heredoc_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	input = NULL;
	ft_signal(HEREDOC); //corrigir
	if (line == 0)
		line = 1;
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
			ft_stderror("warning: here-document at line %d delimited by end-of-file (wanted `%s')", line, limiter);
			//atualizar funcao para receber %d
			ft_exit_status(0, TRUE, TRUE);
		}
		if (ft_strlen(input) != 0 && !ft_strncmp(limiter, input, ft_strlen(input)))
		{
			free(input);
			break ;
		}
		//verificar se realmente precisa salvar o historico
		ft_putendl_fd(input, fd_write);
		line++;
	}
	close(fd_write);
	//return (fd_read = open("/tmp/.heredoc_tmp", O_RDONLY));
	return (0);
}

int	heredoc_fd(char *limiter)
{
	int	fd;

	fd = read_heredoc(limiter);
	ft_signal(DEFAULT);
	//unlink("/tmp/.heredoc_tmp");
	return (ft_exit_status(0, TRUE, TRUE), fd);
}
