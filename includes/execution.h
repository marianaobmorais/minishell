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

/* Heredoc */

void	ft_handle_quotes_hd(char **new_value, char *value, int *i, char **envp);
char	*ft_expand_input(char *input, char **envp);
char	*ft_create_pathname(void);
int		count_line(int mode);
void	ft_search_heredoc(void *curr_node, t_shell *sh);

/* Redir */

int		ft_redir(t_redir *node, void *next_node, t_shell *sh);
//int		ft_open(int type, char *pathname, int mode, t_shell *sh);

/* Launcher */

void	ft_launcher(void *curr_node, void *next_node, int *curr_fds, t_shell *sh);
int		ft_single_command(void *node, void *next_node, t_shell *sh);
void	ft_restore_original_fds(t_shell *sh);
void	ft_save_original_fds(t_shell *sh);
void	close_original_fds(t_shell *sh);
void	ft_launcher_manager(void *tree, t_shell *sh);

/* Exec */

char	**tokentostring(t_list **args);
char	**ft_add_to_vector(char **old_vector, char *new_str);
char	**ft_split_argv(char **args);
void	ft_exec(t_list **args, t_shell *sh);
void	close_fds(int *fds);

#endif //EXECUTION_H