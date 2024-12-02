#include "../../includes/minishell.h"

pid_t	ft_child_process(int *curr_fds, t_shell *sh, void *curr_node, void *next_node, t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_stderror(TRUE, "");
		ft_exit_status(1, TRUE, FALSE);
	}
	if (pid == 0)
	{
		close(curr_fds[0]);
		if (next_node != NULL && (sh->prev && ((t_redir *)sh->prev)->type
			!= OUTFILE) && ((t_redir *)sh->prev)->type != APPEND)
		{
			dup2(curr_fds[1], STDOUT_FILENO);
		}
		close(curr_fds[1]);
		ft_exec(((t_exec *)curr_node)->args, env, sh);
	}
	return (pid);
}

void	ft_parent_process(void *node, t_env *env, pid_t pid, int *curr_fds, t_shell *sh)
{
	int	status;
	static int	i; //remover

	if (!node) //entra apenas no ultimo validar se funciona o signal
	{
		ft_signal(CHILD_);
		if (waitpid(pid, &status, 0) != -1) //esperando demais para um |, vai mudar com && e ||
		{
			if (WIFEXITED(status))
				ft_exit_status(WEXITSTATUS(status), TRUE, FALSE); // grava exit status
			else if (WIFSIGNALED(status))
			{
				ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE); // grava exit status SOMAR 128
				// talvez da um return -1 para encerrar a funcao launcher e volta ao cli
			}
		}
	}
	ft_stderror(FALSE, "[%d] PID = %d, Status = %d", i++, pid, ft_exit_status(0, FALSE, FALSE)); //remover
	if (curr_fds)
	{
		close(curr_fds[1]);
		if (node)
			dup2(curr_fds[0], STDIN_FILENO);
		close(curr_fds[0]);
	}
	if (!node)
	{
		ft_restore_original_fds(sh);
		return ;
	}
	if (sh->prev && (((t_redir *)sh->prev)->type == OUTFILE
		|| ((t_redir *)sh->prev)->type == APPEND))
		dup2(sh->stdout_, STDOUT_FILENO); // restaurar
	if (node)
		ft_launcher(node, ((t_pipe *)node)->right, env, NULL, sh);
}

void	ft_launcher(void *curr_node, void *next_node, t_env *env, int *curr_fds, t_shell *sh)
{
	pid_t	pid;
	int		fds[2];

	ft_save_original_fds(sh);
	if (!curr_node)
		return;
	else if (((t_pipe *)curr_node)->type == PIPE)
	{
		sh->prev = curr_node;
		ft_launcher(((t_pipe *)curr_node)->left, ((t_pipe *)curr_node)->right, env, fds, sh);
	}
	else if (ft_redir(((t_redir *)curr_node), *(env)->global, sh))
	{
		sh->prev = curr_node;
		ft_launcher(((t_redir *)curr_node)->next, next_node, env, curr_fds, sh);
	}
	else if (((t_exec *)curr_node)->type == EXEC)
	{
		if (pipe(curr_fds) == -1)
		{
			ft_stderror(TRUE, "");
			ft_exit_status(1, TRUE, FALSE);
		}
		pid = ft_child_process(curr_fds, sh, curr_node, next_node, env);
		ft_parent_process(next_node, env, pid, curr_fds, sh);
	}
}
