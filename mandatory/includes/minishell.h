/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:03:04 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/29 22:06:53 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// system libraries
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <limits.h>

// our own libraries
# include "../../libft/libft.h"
# include "builtins.h"
# include "tokenize.h"
# include "execution.h"

//program name
# define PROG_NAME "\001\033[1m\033[31m\002b\001\033[33m\002a\001\033[32m\002s\
\001\033[34m\002h\001\033[35m\002i\001\033[36m\002n\001\033[34m\002h\
\001\033[31m\002o\001\033[0m\002 $ "
# define PROG_NAME_ERROR "\033[1;31mbashinho: \033[0m"

//messages
# define MALLOC "memory allocation failed"
# define ERROR_INIT "arguments are not accepted when initializing the program"

# define TRUE 1
# define FALSE 0

typedef enum e_env
{
	GLOBAL,
	LOCAL,
	LIMBO,
	DEFAULT
}	t_env;

/* ft_free_utils.c */

void	ft_free_vector(char **vector);
void	ft_free_list(t_list *list);
void	ft_free_content(t_list *node);
void	ft_free_sh(t_shell *sh);
void	ft_child_cleaner(t_shell *sh, char **args, int mode);

/* ft_exit_status.c */

int		ft_exit_status(int exit_status, int write_, int exit_);

/* ft_error_handler.c */

void	ft_stderror(int perror_, const char *str, ...);
void	ft_putstr_fd_len(char *s, int fd, int len);
void	ft_error_malloc(char *message);

/* ft_signal.c */

void	ft_signal(int type);

/* ft_signal_utils.c */

void	sig_heredoc_parent(void);
void	sig_heredoc_child(void);
void	ft_print_signal(void);

/* ft_cli.c */

void	ft_cli(t_shell *sh);
t_shell	*ft_init_sh(char **envp);
void	ft_restore_cli(t_shell *sh, void *tree);

/* ft_env_manager.c */

char	**ft_get_my_envp(char **envp);
char	**ft_merge_env(char **env1, char **env2);

/* ft_copy_list.c */

t_list	**ft_copy_list(t_list **old);

/* ft_handle_lvl.c */

void	ft_handle_lvl(t_shell *sh);

#endif //MINISHELL_H
