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

# define PROMPT "\033[47m\033[1;31mb\033[1;33ma\033[1;32ms\033[1;36mh\033[1;34mi\033[1;35mn\033[1;37mh\033[1;30mo\033[1;91m$\033[0m "
# define PROG_NAME "bashinho"
# define PROG_NAME_ERROR "\033[1;31mbashinho: \033[0m"

# define TRUE 1
# define FALSE 0

# define PARENT 0
# define CHILD 1
# define HEREDOC 2
# define DEFAULT 3

typedef struct s_bonsai
{
	t_token		*left;
	t_token		*right;
	t_type	type;
	t_state	state;
	bool	expand;

}	t_bonsai;

int		ft_error_handler(void);
char	**ft_get_my_envp(char **envp);

//ft_free_utils.c
void	ft_free_vector(char **vector);
void	ft_free_list(t_list **list);

int		ft_exit_status(int exit_status, int write_, int exit_);
void	ft_stderror(const char *str, ...);
void	ft_signal(int type);
void	ft_cli(char ***envp);
void	ft_launcher(t_bonsai *bonsai, char ***my_envp);

int		heredoc_fd(char *limiter);



#endif //MINISHELL_H