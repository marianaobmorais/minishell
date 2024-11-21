#include "../includes/minishell.h"

// pid_t	ft_child_process(int *fds)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == -1)
// 		ft_error_handler(); //tratar
// 	if (pid == 0)
// 	{
// 		close(fds[0]);
// 		dup2(fds[1], STDOUT_FILENO);
// 		close(fds[1]);
// 	}
// 	return (pid);
// }

// void	ft_parent_process(pid_t pid, int *fds)
// {
// 	int		status;

// 	printf("Dentro do Parent process\n");
// 	if (waitpid(pid, &status, 0) != -1)
// 	{
// 		if (WIFEXITED(status))
// 			ft_exit_status(WEXITSTATUS(status), TRUE, FALSE); //grava exit status
// 		else if (WIFSIGNALED(status))
// 		{
// 			ft_signal(CHILD);
// 			ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE); //grava exit status
// 			//talvez da um return -1 para encerrar a funcao launcher e volta ao cli
// 		}
// 		close(fds[1]);
// 		dup2(fds[0], STDIN_FILENO);
// 		close(fds[0]);
// 	}
// }

// void	ft_launcher(t_bonsai *bonsai, char ***my_envp) //receber tree
// {
// 	pid_t	pid;
// 	int		fds[2];

// 	pid = -1; //sinaliza que nao tem child
// 	if (!bonsai)
// 		return ;
// 	if (bonsai->type == PIPE)
// 	{
// 		if (pipe(fds) == -1)
// 			ft_error_handler(); //tratar
// 		pid = ft_child_process(fds);
// 		ft_launcher(bonsai->left, my_envp); //left | Termina apenas no ultimo no da esquerda
// 		ft_parent_process(pid, fds);
// 		ft_launcher(bonsai->right, my_envp); //right
// 	}
// 	if (bonsai->type == OUTFILE)
// 		printf("outfile\n");
// 	if (bonsai->type == INFILE)
// 		printf("infile\n");
// 	if (bonsai->type == HEREDOC)
// 		printf("heredoc\n");
// 	if (bonsai->type == EXEC)
// 	{
// 		if (pid == -1)
// 			printf("Nao existe child\n");
// 		printf("exec\n");
// 		//ft_exec(stuffs, pid);
// 	}
// 	bonsai->left;
// }