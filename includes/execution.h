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

void	ft_launcher(void *curr_node, void *next_node, t_env *env, int *curr_fds);
void	ft_exec(t_list **args, t_env *env);
int		ft_redir(t_redir *node, char **my_envp);
int		heredoc_fd(char *limiter, char **my_envp, int state);
int		ft_single_command(void *node, t_env *env);

#endif //EXECUTION_H