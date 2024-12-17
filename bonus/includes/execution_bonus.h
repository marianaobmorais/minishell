/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:43:17 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:43:18 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_BONUS_H
# define EXECUTION_BONUS_H

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
	t_node	*prev;
	t_list	**heredoc_list;
	char	**global;
	char	**local;
	char	**limbo;
	char	*prompt;
	int		run;
	int		search_heredoc;
	t_node	*next_node;
	t_node	*prev_nnode;
	void	*root;
}	t_shell;

/* ft_heredoc_utils_bonus.c */

void	ft_handle_quotes_hd(char **new_value, char *value, int *i, char **envp);
char	*ft_expand_input(char *input, char **envp);
char	*ft_create_pathname(void);
int		count_line(int mode);
void	ft_heredoc_manager(void *tree, t_shell *sh);

/* ft_heredoc_bonus.c */

void	ft_search_heredoc(t_node *curr_node, t_shell *sh);

/* ft_redir_bonus.c */

int		ft_redir(t_redir *nd, t_shell *sh);

/* ft_launcher_bonus.c */

void	ft_launcher(t_node *curr_node, t_node *next_node, int *curr_fds, \
	t_shell *sh);
void	ft_launcher_manager(void *tree, t_shell *sh);

/* ft_launcher_utils_bonus.c */

void	ft_restore_original_fds(t_shell *sh);
void	ft_save_original_fds(t_shell *sh);
void	close_original_fds(t_shell *sh);
void	close_fds(int *fds);
void	ft_issubroot(t_node *node, t_shell *sh);

/* ft_exec_bonus.c */

int		ft_exec(t_list **args, t_shell *sh);

/* ft_exec_utils_bonus.c */

char	**tokentostring(t_list **args);
int		ft_single_command(t_node *node, t_shell *sh); //check later
char	**ft_add_to_vector(char **old_vector, char *new_str);

/* ft_exec_ultimate_bonus.c */

int		ft_free_paths(char **paths, int i);
int		isvalid_dir(char *pathname, char **args, t_shell *sh);
int		isvalid_file(char *pathname, char **args, t_shell *sh);
char	*merge(char *s1, char *s2);

#endif //EXECUTION_BONUS_H