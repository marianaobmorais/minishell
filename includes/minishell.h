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

// our own libraries
# include "../libft/libft.h"
# include "builtins.h"
# include "tokenize.h"

//program name
# define PROMPT "bashinho [pwd] $ "
# define PROG_NAME "bashinho"
# define PROG_NAME_ERROR "\033[1;31mbashinho: \033[0m"

# define TRUE 1
# define FALSE 0

int		ft_error_handler(void);
char	**ft_get_my_envp(char **envp);

//ft_free_utils.c
void	ft_free_vector(char **vector);
void	ft_free_list(t_list **list);
void	ft_free_node(t_list *node);

int		ft_exit_status(int status, int write_, int exit_);
void	ft_stderror(int perror_, const char *str, ...);
void	ft_signal_parent(void);
void	ft_signal_child(void);
void	ft_cli(char ***envp);

#endif //MINISHELL_H