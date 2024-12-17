/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:42:59 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:43:00 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int		fds[2];
	void	*prev;
	t_list	**heredoc_list;
	char	**global;
	char	**local;
	char	**limbo;
	char	*prompt;
	int		run;
	void	*root;
}	t_shell;

/* ft_heredoc_utils.c */

void	ft_handle_quotes_hd(char **new_value, char *value, int *i, char **envp);
char	*ft_expand_input(char *input, char **envp);
char	*ft_create_pathname(void);
int		count_line(int mode);

/* ft_heredoc.c */

void	ft_search_heredoc(void *curr_node, t_shell *sh);

/* ft_redir.c */

int		ft_redir(t_redir *nd, t_shell *sh);
//int		ft_open(int type, char *pathname, int mode, t_shell *sh);

/* ft_launcher.c */

void	ft_launcher(void *curr_node, void *next_node, int *curr_fds, \
	t_shell *sh);
void	ft_launcher_manager(void *tree, t_shell *sh);

/* ft_launcher_utils.c */

int		ft_single_command(void *node, t_shell *sh);
void	ft_restore_original_fds(t_shell *sh);
void	ft_save_original_fds(t_shell *sh);
void	close_original_fds(t_shell *sh);
void	close_fds(int *fds);

/* ft_exec.c */

int		ft_exec(t_list **args, t_shell *sh);

/* ft_exec_utils.c */

char	**tokentostring(t_list **args);
char	**ft_tovector(char **old_vector, char *new_str);

/* ft_exec_ultimate.c */

int		ft_free_paths(char **paths, int i);
int		isvalid_dir(char *pathname, char **args, t_shell *sh);
int		isvalid_file(char *pathname, char **args, t_shell *sh);
char	*merge(char *s1, char *s2);

#endif //EXECUTION_H