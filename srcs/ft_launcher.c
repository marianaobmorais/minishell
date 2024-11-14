#include "../includes/minishell.h"

void	ft_child_process(int *fds)
{
	pid_t	pid;

	pid = fork()
	if (pid == -1)
		//tratar
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);

	}
}

void	ft_parent_process(void)
{
	if (waitpid(pid, &status, 0) != -1)
	{
		if (WIFEXITED(status))
			ft_exit_status(WEXITSTATUS(status), TRUE, FALSE); //grava exit status
		else if (WIFSIGNALED(status))
		{
			ft_signal(CHILD);
			ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE); //grava exit status
		}
	}
}

void	ft_launcher(t_bonsai bonsai, char ***my_envp) //receber tree
{
	pid_t	pid;
	int		status;

	if (!bonsai)
		return ;
	if (bonsai->type == PIPE)
	{
		if (pipe(fds) == -1)
			//tratar
		ft_launcher_(stuffs); //left
		ft_parent_process();
		ft_launcher_(stuffs); //right
	}
	if (bonsai->type == REDIR)
	{
		if (OUTFILE) //> or >>

		if (INFILE) // <

		if (HEREDOC) // <<
	}
	if (bonsai->type == EXEC)
		ft_exec(stuffs);
	bonsai->next;
}
