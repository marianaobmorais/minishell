#ifndef EXECUTION_H
#define EXECUTION_H

void	ft_launcher(void *curr_bonsai, void *next_bonsai, char ***my_envp, int *fds_current);
int		heredoc_fd(char *limiter, char **my_envp, int expand);
void	ft_exec(char **args, char **my_envp);
int		ft_redir(t_redir *node, char **my_envp);

#endif //EXECUTION_H