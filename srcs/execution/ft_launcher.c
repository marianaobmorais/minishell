#include "../../includes/minishell.h"

pid_t ft_child_process(void *node, char ***my_envp, int *curr_fds)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
		ft_stderror("erro no fork"); // tratar
	if (pid == 0)
	{
		ft_signal(CHILD_);
		ft_launcher(((t_pipe *)node)->left, ((t_pipe *)node)->right, my_envp, curr_fds); // left | Termina apenas no ultimo no da esquerda
		ft_exit_status(0, TRUE, TRUE);
	}
	return (pid);
}

void ft_parent_process(void *node, char ***my_envp, pid_t pid, int *curr_fds)
{
	int status;
	static int	i; //remover

	status = 999; // fora do range para assegura que seja um valor valido
	if (!((t_pipe *)node)->right) //entra apenas no ultimo validar se funciona o signal
	{
		if (waitpid(pid, &status, 0) != -1) //esperando demais para um |, vai mudar com && e ||
		{
			if (WIFEXITED(status))
				ft_exit_status(WEXITSTATUS(status), TRUE, FALSE); // grava exit status
			else if (WIFSIGNALED(status))
			{
				ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE); // grava exit status
				// talvez da um return -1 para encerrar a funcao launcher e volta ao cli
			}
		}
	}
	printf("\n[%d] PID = %d, Status = %d\n", i++, pid, ft_exit_status(0, FALSE, FALSE)); //remover
	if (curr_fds)
	{
		close(curr_fds[1]);
		dup2(curr_fds[0], STDIN_FILENO);
		close(curr_fds[0]);
	}
	if (((t_pipe *)node)->right)
		ft_launcher(((t_pipe *)node)->right, NULL, my_envp, NULL);
}

void ft_launcher(void *curr_node, void *next_node, char ***my_envp, int *curr_fds)
{
	pid_t	pid;
	int		fds[2];
	int		original_stdin;

	original_stdin = dup(STDIN_FILENO);
	if (!curr_node)
		return;
	else if (((t_pipe *)curr_node)->type == PIPE)
	{
		if (pipe(fds) == -1)
			ft_error_handler(); // tratar
		pid = ft_child_process(curr_node, my_envp, fds);
		ft_parent_process(curr_node, my_envp, pid, fds);
	}
	else if (ft_redir(((t_redir *)curr_node), *my_envp))
		ft_launcher(((t_redir *)curr_node)->next, next_node, my_envp, curr_fds);
	else if (((t_exec *)curr_node)->type == EXEC)
	{
		ft_signal(CHILD_);
		close(curr_fds[0]);
		if (next_node != NULL)
			dup2(curr_fds[1], STDOUT_FILENO);
		close(curr_fds[1]);
		ft_exec(((t_exec *)curr_node)->args, *my_envp);
	}
	else
		ft_putstr_fd("Perdeu a linha\n\n", 2);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}