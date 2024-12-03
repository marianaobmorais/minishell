#include "../../includes/minishell.h"

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

static void	save_heredoc(char *pathname, int fd, t_shell *sh)
{
	int		fd_save;
	int		rd;
	char	*buffer;

	fd_save = open(pathname, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_save == -1)
	{
		ft_stderror(TRUE, "");
		ft_exit_status(1, TRUE, FALSE);
		return ;
	}
	buffer = malloc(BUFFER_SIZE * sizeof(BUFFER_SIZE));
	rd = 1;
	while (rd != 0)
	{
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd <= 0)
			break ;
		write(fd_save, buffer, ft_strlen(buffer));
		free(buffer);
	}
	close(fd);
	close(fd_save);
	ft_lstadd_back(sh->heredoc_list, ft_lstnew(ft_strdup(pathname)));
	free(pathname);
}

static void	wait_heredoc(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) != -1)
	{
		if (WIFEXITED(status))
			ft_exit_status(WEXITSTATUS(status), TRUE, FALSE);
		else if (WIFSIGNALED(status))
			ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE);
	}
}

static int	heredoc_fd(char *eof, char **my_envp, t_state state, t_shell *sh)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), FALSE);
	pid = fork();
	if (pid == -1)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), FALSE);
	if (pid == 0)
	{
		close(fd[0]);
		read_heredoc(eof, state, my_envp, fd[1]);
		close(fd[0]);
		ft_exit_status(0, TRUE, TRUE);
	}
	close(fd[1]);
	wait_heredoc(pid);
	if (ft_exit_status(0, FALSE, FALSE) != 0)
		return (FALSE);
	save_heredoc(ft_create_pathname(), fd[0], sh);
	close(fd[0]);
	return (TRUE);
}

void	ft_search_heredoc(void *node, t_shell *sh)
{
	t_redir	*rnode;
	t_token	*tnode;
	t_state	state;

	if (!node)
		return ;
	else if (((t_node *)node)->type == PIPE)
	{
		ft_search_heredoc(((t_node *)node)->left, sh);
		ft_search_heredoc(((t_node *)node)->right, sh);
		return ;
	}
	else if (((t_redir *)node)->type == HEREDOC && sh->run == TRUE)
	{
		rnode = (t_redir *)node;
		ft_process_token_list(rnode->target, ft_merge_env(sh)); //ft_merge_env
		tnode = (t_token *)(*rnode->target)->content;
		state = tnode->state;
		sh->run = heredoc_fd(tnode->value, ft_merge_env(sh), state, sh); //ft_merge_env
	}
	else if (((t_redir *)node)->type == EXEC
		|| ((t_redir *)node)->type == EXPORT
		|| ((t_redir *)node)->type == EXPORT_AP)
		return ;
	ft_search_heredoc(((t_redir *)node)->next, sh);
}
