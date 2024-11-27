#ifndef EXECUTION_H
# define EXECUTION_H

typedef enum e_signal
{
	PARENT_,
	CHILD_,
	HEREDOC_,
	DEFAULT_
}	t_signal;

typedef struct s_env
{
	char	***global;
	char	***local;
}	t_env;

typedef struct s_shell
{
	int	stdin_;
	int	stdout_;
	int	fds_saved;
	void *curr;
	void *next;
	void *prev;
	int heredoc;
}	t_shell;

void	ft_launcher(void *curr_node, void *next_node, t_env *env, int *curr_fds, t_shell *sh);
void	ft_exec(t_list **args, t_env *env,t_shell *sh);
int		ft_redir(t_redir *node, char **my_envp, int *fds,t_redir *next_node, t_shell *sh);
int		heredoc_fd(char *eof, char **my_envp, int state);
int		ft_single_command(void *node, t_env *env, t_shell *sh);
char	**tokentostring(t_list **args);

#endif //EXECUTION_H