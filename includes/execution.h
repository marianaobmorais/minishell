#ifndef EXECUTION_H
# define EXECUTION_H

typedef enum e_signal
{
	PARENT_,
	CHILD_,
	HEREDOC_,
	DEFAULT_
}	t_signal;

typedef struct s_shell
{
	int		stdin_;
	int		stdout_;
	int		stderr_;
	int		fds_saved;
	int		error_fd;
	void	*prev;
	t_list	**heredoc_list;
	char	**global;
	char	**local;
	char	**limbo;
	int		run;
	void	*root;
}	t_shell;

//ft_heredoc_utils.c

void	ft_handle_quotes_hd(char **new_value, char *value, int *i, char **envp);
char	*ft_expand_input(char *input, char **envp);
char	*ft_create_pathname(void);
int		count_line(int mode);

//ft_heredoc.c

void	ft_search_heredoc(void *curr_node, t_shell *sh);

//ft_redir.c

int		ft_redir(t_redir *node, void *next_node, t_shell *sh);
//int		ft_open(int type, char *pathname, int mode, t_shell *sh);

//ft_launcher.c
void	ft_launcher(void *curr_node, void *next_node, int *curr_fds, \
	t_shell *sh);
void	ft_launcher_manager(void *tree, t_shell *sh);

//ft_launcher_utils.c
int		ft_single_command(void *node, void *next_node, t_shell *sh);
void	ft_restore_original_fds(t_shell *sh);
void	ft_save_original_fds(t_shell *sh);
void	close_original_fds(t_shell *sh);
void	close_fds(int *fds);

//ft_exec.c
void	ft_exec(t_list **args, t_shell *sh);
char	**ft_add_to_vector(char **old_vector, char *new_str);
char	*merge(char *s1, char *s2);

//ft_exec_utils.c
char	**tokentostring(t_list **args);
char	**ft_split_argv(char **args);


#endif //EXECUTION_H