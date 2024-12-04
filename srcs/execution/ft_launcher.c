#include "../../includes/minishell.h"

/**
 * @brief Forks a child process to execute a command with redirection.
 *
 * Creates a child process using fork, sets up file descriptor redirections,
 * and executes the command. Handles errors and manages the shell state.
 *
 * @param fds An array of file descriptors for the pipe.
 * @param sh The shell structure containing the execution state and environment
 * @param node The current command node to be executed.
 * @param next_node The next command node in the pipeline.
 *
 * @return The process ID of the child process.
 */
pid_t	ft_child_process(int *fds, t_shell *sh, void *node, void *next_node)
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
		close(fds[0]);
		if (next_node != NULL && (sh->prev && ((t_redir *)sh->prev)->type
				!= OUTFILE) && ((t_redir *)sh->prev)->type != APPEND)
		{
			dup2(fds[1], STDOUT_FILENO);
		}
		close(fds[1]);
		ft_exec(((t_exec *)node)->args, sh);
	}
	return (pid);
}

/**
 * @brief Manages the parent process in a pipeline or command execution.
 *
 * Handles signals, waits for the child process to finish, and updates the
 * exit status. Manages file descriptors for pipeline continuation or restores
 * the original file descriptors if the pipeline ends.
 *
 * @param curr_fds The current file descriptors for the pipe.
 * @param sh The shell structure containing the execution state and environment
 * @param node The current node in the command pipeline.
 * @param pid The process ID of the child process.
 */
void	ft_parent_process(int *curr_fds, t_shell *sh, void *node, pid_t pid)
{
	int	status;

	if (!node)
	{
		ft_signal(CHILD_);
		if (waitpid(pid, &status, 0) != -1)
		{
			if (WIFEXITED(status))
				ft_exit_status(WEXITSTATUS(status), TRUE, FALSE);
			else if (WIFSIGNALED(status))
				ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE);
		}
	}
	if (curr_fds)
	{
		close(curr_fds[1]);
		if (node)
			dup2(curr_fds[0], STDIN_FILENO);
		close(curr_fds[0]);
	}
	if (!node)
		return (ft_restore_original_fds(sh));
	if (sh->prev && (((t_redir *)sh->prev)->type == OUTFILE
			|| ((t_redir *)sh->prev)->type == APPEND))
		dup2(sh->stdout_, STDOUT_FILENO);
	if (node)
		ft_launcher(node, ((t_node *)node)->right, NULL, sh);
}

/**
 * @brief Launches commands in a pipeline or executes redirections.
 *
 * Recursively processes a command node and its next node, handling
 * pipes and redirections. Forks child processes to execute commands
 * and manages parent processes.
 *
 * @param node The current command node to be processed.
 * @param next_node The next command node in the pipeline.
 * @param curr_fds The current file descriptors for the pipe.
 * @param sh The shell structure containing the execution state and environment
 */
void	ft_launcher(void *node, void *next_node, int *curr_fds, t_shell *sh)
{
	pid_t	pid;
	int		fds[2];

	ft_save_original_fds(sh);
	if (!node)
		return ;
	else if (((t_node *)node)->type == PIPE)
	{
		sh->prev = node;
		ft_launcher(((t_node *)node)->left, ((t_node *)node)->right, fds, sh);
	}
	else if (ft_redir(((t_redir *)node), sh))
	{
		sh->prev = node;
		ft_launcher(((t_redir *)node)->next, next_node, curr_fds, sh);
	}
	else if (((t_exec *)node)->type == EXEC)
	{
		if (pipe(curr_fds) == -1)
			return (ft_exit_status(1, TRUE, FALSE), ft_stderror(TRUE, ""));
		pid = ft_child_process(curr_fds, sh, node, next_node);
		ft_parent_process(curr_fds, sh, next_node, pid);
	}
}
