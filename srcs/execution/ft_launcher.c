#include "../../includes/minishell.h"

// static void	ft_restore_fd(int fd_stdin, int fd_stdout)
// {
// 	if (dup2(fd_stdin, STDIN_FILENO) == -1)
// 		ft_error_handler();
// 	if (dup2(fd_stdout, STDOUT_FILENO) == -1)
// 		ft_error_handler();
// 	close(fd_stdin);
// 	close(fd_stdout);
// }

pid_t	ft_child_process(void *bonsai, char ***my_envp, int *fds_current)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_error_handler(); //tratar
	if (pid == 0)
	{
		ft_signal(CHILD_);
		ft_putstr_fd("Dentro de Child\n", 2);
		ft_launcher(((t_pipe *)bonsai)->left, ((t_pipe *)bonsai)->right, my_envp, fds_current); //left | Termina apenas no ultimo no da esquerda
		exit(0);
	}
	return (pid);
}

void	ft_parent_process(void *bonsai, char ***my_envp, pid_t pid, int *fds_current)
{
	int		status;

	if (waitpid(pid, &status, 0) != -1)
	{
		if (WIFEXITED(status))
			ft_exit_status(WEXITSTATUS(status), TRUE, FALSE); //grava exit status
		else if (WIFSIGNALED(status))
		{
			ft_signal(CHILD_);
			ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE); //grava exit status
			//talvez da um return -1 para encerrar a funcao launcher e volta ao cli
		}
	}
	ft_signal(PARENT_);
	if (fds_current)
	{
		close(fds_current[1]);
		dup2(fds_current[0], STDIN_FILENO);
		close(fds_current[0]);
	}
	if (((t_pipe *)bonsai)->right)
		ft_launcher(((t_pipe *)bonsai)->right, NULL, my_envp, NULL);
}

void	ft_launcher(void *curr_bonsai, void *next_bonsai, char ***my_envp, int *fds_current) //receber tree
{
	pid_t	pid;
	int		fds[2];

	if (!curr_bonsai)
		return ;
	else if (((t_pipe *)curr_bonsai)->type == PIPE)
	{
		if (pipe(fds) == -1)
			ft_error_handler(); //tratar
		pid = ft_child_process(curr_bonsai, my_envp, fds);
		ft_parent_process(curr_bonsai, my_envp, pid, fds);
	}
	// else if (((t_redir *)curr_bonsai)->type == OUTFILE)
	// {
	// 	ft_putstr_fd("outfile\n", 2); //fazer funcao para receber e tratar cada um
	// 	ft_launcher(((t_pipe *)bonsai)->left, my_envp);
	// }
	else if (((t_exec *)curr_bonsai)->type == EXEC)
	{
		close(fds_current[0]);
		if (next_bonsai == NULL)
			close(fds_current[1]);
		else
			dup2(fds_current[1], STDOUT_FILENO);
		close(fds_current[1]);
		ft_exec(((t_exec *)curr_bonsai)->args, *my_envp);
	}
	else
		ft_putstr_fd("Perdeu a linha\n\n", 2);
}
