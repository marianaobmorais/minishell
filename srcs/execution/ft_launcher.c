#include "../../includes/minishell.h"


void ft_save_original_fds(t_shell *sh) 
{
	sh->stdin_ = dup(STDIN_FILENO);
	sh->stdout_ = dup(STDOUT_FILENO);
	if (sh->stdin_ == -1 || sh->stdout_ == -1) {
		perror("Error saving original FDs");
		exit(EXIT_FAILURE);
	}
}

void ft_restore_original_fds(t_shell *sh)
{

	if (dup2(sh->stdin_, STDIN_FILENO) == -1 || dup2(sh->stdout_, STDOUT_FILENO) == -1) {
		perror("Error restoring original FDs");
	}
	close(sh->stdin_);
	close(sh->stdout_);
}

int	ft_single_command(void *node, t_env *env, t_shell *sh)
{
	void	*curr_node;
	char	**new_args;

	if (ft_isjustbuiltin(node, env))
	{
		if (!sh->fds_saved) {
		    ft_save_original_fds(sh);
		    sh->fds_saved = 1;
		}
		curr_node = ((t_pipe *)node)->left;
		while (ft_redir(((t_redir *)curr_node), *(env)->global, NULL, NULL, NULL))
			curr_node = ((t_redir *)curr_node)->next;
		if (((t_exec *)curr_node)->type == EXEC)
		{
			new_args = tokentostring(((t_exec *)curr_node)->args);
			if (ft_isbuiltin(new_args))
				ft_exec_builtin(new_args, env);
			ft_free_vector(new_args);
		}
		ft_restore_original_fds(sh);
		return (TRUE);
	}
	return (FALSE);
}

// pid_t ft_child_process(void *node, t_env *env, int *curr_fds, t_shell *sh)
// {
// 	pid_t pid;

// 	pid = fork();
// 	if (pid == -1)
// 		ft_stderror(TRUE, ""); // tratar encerrar minishell ou apenas falhar linha de comando
// 	if (pid == 0)
// 	{
// 		ft_signal(CHILD_);
// 		ft_launcher(((t_pipe *)node)->left, ((t_pipe *)node)->right, env, curr_fds, sh); // left | Termina apenas no ultimo no da esquerda
// 		ft_exit_status(0, TRUE, TRUE); // exit com exit status da finalizacao
// 	}
// 	return (pid);
// }


// void ft_launcher(void *curr_node, void *next_node, t_env *env, int *curr_fds, t_shell *sh)
// {
// 	pid_t	pid;
// 	int		fds[2];
// 	int		original_fd[2];

// 	original_fd[0] = dup(STDIN_FILENO);
// 	original_fd[1] = dup(STDOUT_FILENO);
// 	if (!curr_node)
// 		return;
// 	else if (((t_pipe *)curr_node)->type == PIPE)
// 	{
// 		if (pipe(fds) == -1)
// 			ft_error_handler(); // tratar
// 		pid = ft_child_process(curr_node, env, fds, sh);
// 		ft_parent_process(curr_node, env, pid, fds, sh);
// 	}
// 	else if (ft_redir(((t_redir *)curr_node), *(env)->global, original_fd, NULL))
// 		ft_launcher(((t_redir *)curr_node)->next, next_node, env, curr_fds, sh);
// 	else if (((t_exec *)curr_node)->type == EXEC)
// 	{
// 		ft_signal(CHILD_);
// 		close(curr_fds[0]);
// 		if (next_node != NULL) //ha mais comandos para serem executados
// 			dup2(curr_fds[1], STDOUT_FILENO); // a saida do comando vai ser gravada no pipe fd 1
// 		close(curr_fds[1]);
// 		ft_exec(((t_exec *)curr_node)->args, env);
// 	}
// 	else
// 		ft_putstr_fd("Perdeu a linha\n\n", 2);
// 	dup2(original_fd[1], STDOUT_FILENO);
// 	dup2(original_fd[0], STDIN_FILENO);
// 	close(original_fd[0]);
// 	close(original_fd[1]);
// }

// void ft_launcher(void *curr_node, void *next_node, t_env *env, int *curr_fds, t_shell *sh)
// {
// 	pid_t	pid;
// 	int		fds[2];

// 	if (sh->fds_saved == 0) {
//         ft_save_original_fds(sh);
//         sh->fds_saved = 1;
//     }

// 	if (!curr_node)
// 		return;
// 	else if (((t_pipe *)curr_node)->type == PIPE)
// 	{
// 		if (pipe(fds) == -1)
// 			ft_error_handler(); // tratar
// 		pid = ft_child_process(curr_node, env, fds, sh);
// 		ft_parent_process(curr_node, env, pid, fds, sh);
// 	}
// 	else if (((t_pipe *)curr_node)->type != PIPE)
// 	{
// 		ft_redir(((t_redir *)curr_node), *(env)->global, NULL, ((t_redir *)curr_node)->next);

// 		// if (((t_exec *)curr_node)->type != EXEC
// 		// 	&& ((t_redir *) ((t_redir *)curr_node)->next)->type == HEREDOC)
// 		// {
// 		// 	ft_putendl_fd("restaurando no redir", 2);
// 		// 	ft_restore_original_fds(sh);
// 		// }

// 		if (((t_exec *)curr_node)->type == EXEC)
// 		{
// 			ft_signal(CHILD_);
// 			close(curr_fds[0]);
// 			if (next_node != NULL && (sh->prev && ((t_redir *)sh->prev)->type != OUTFILE))
// 				dup2(curr_fds[1], STDOUT_FILENO);
// 			close(curr_fds[1]);
// 			ft_exec(((t_exec *)curr_node)->args, env);
// 		}
// 		//ft_restore_original_fds(sh);
// 		sh->prev = curr_node;
// 		ft_stderror(FALSE, "save prev %d", ((t_redir *)sh->prev)->type);
// 		ft_launcher(((t_redir *)curr_node)->next, next_node, env, curr_fds, sh);
// 	}
// 	if (!next_node) {
//         ft_restore_original_fds(sh);
//     }
// 	// ft_restore_original_fds(sh);
// }

void ft_parent_process(void *node, t_env *env, pid_t pid, int *curr_fds, t_shell *sh)
{
	int status;
	static int	i; //remover

	status = 999; // fora do range para assegura que seja um valor valido
	//close(curr_fds[1]);
	//if (!node) //entra apenas no ultimo validar se funciona o signal
	if (!node || ft_isheredoc(node)) //entra apenas no ultimo validar se funciona o signal
	{
		ft_signal(CHILD_);
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
	if (sh->prev && (((t_redir *)sh->prev)->type == OUTFILE || ((t_redir *)sh->prev)->type == APPEND))
	{
		ft_putendl_fd("restaurando stdout" ,2);
		dup2(sh->stdout_, STDOUT_FILENO); // restaurar
	}
	if (node)
		ft_launcher(node, ((t_pipe *)node)->right, env, NULL, sh);
}

void ft_launcher(void *curr_node, void *next_node, t_env *env, int *curr_fds, t_shell *sh)
{
	pid_t	pid;
	int fds[2];

	if (sh->fds_saved == 0) {
		ft_save_original_fds(sh);
		sh->fds_saved = 1;
	}
	if (!curr_node)
		return;
	else if (((t_pipe *)curr_node)->type == PIPE)
	{
		sh->prev = curr_node;
		ft_launcher(((t_pipe *)curr_node)->left, ((t_pipe *)curr_node)->right, env, fds, sh);
	}
	else if (ft_redir(((t_redir *)curr_node), *(env)->global, NULL, ((t_redir *)curr_node)->next, sh))
	{
		//ft_redir(((t_redir *)curr_node), *(env)->global, NULL, ((t_redir *)curr_node)->next); //organizar args
		sh->prev = curr_node;
		ft_launcher(((t_redir *)curr_node)->next, next_node, env, curr_fds, sh);
	}
	else/*  if (((t_exec *)curr_node)->type == EXEC) */
	{
		if (pipe(curr_fds) == -1)
			ft_stderror(TRUE, "PIPE PIPE PIPE"); // tratar
		pid = fork();
		if (pid == -1)
			ft_stderror(TRUE, "PID PID PID"); // tratar encerrar minishell ou apenas falhar linha de comando
		if (pid == 0)
		{
			t_redir *node = curr_node;
			if (node->type == OUTFILE || node->type == INFILE || node->type == APPEND)
			{
				ft_process_token_list(node->target,*env->global);
				int fd = ft_open(node->type, ((t_token *)(*node->target)->content)->value, node->mode);
				if (fd == -1)
					return /* (FALSE) */;
				if (node->type == OUTFILE || node->type == APPEND)
					dup2(fd, STDOUT_FILENO);
				else
					dup2(fd, STDIN_FILENO);
				close(fd);
				//return (TRUE);
			}
			close(curr_fds[0]);
			//if (next_node != NULL)
			if (next_node != NULL && (sh->prev && ((t_redir *)sh->prev)->type != OUTFILE) && ((t_redir *)sh->prev)->type != APPEND)
			{
				ft_putendl_fd("redirecionado para o parent" ,2);
				dup2(curr_fds[1], STDOUT_FILENO);
			}
			close(curr_fds[1]);
			ft_exec(((t_exec *)curr_node)->args, env, sh);
		}
		ft_parent_process(next_node, env, pid, curr_fds, sh);
	}
}
