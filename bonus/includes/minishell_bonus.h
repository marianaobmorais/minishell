#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

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
# include "builtins_bonus.h"
# include "tokenize_bonus.h"
# include "execution_bonus.h"
# include "colors_bonus.h"

//program name
# define PROMPT "bashinho [pwd] $ "
# define PROG_NAME "bashinho"
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
void	ft_error_malloc(char *message);

/* ft_signal.c */

void	ft_signal(int type);

/* ft_cli.c */

void	ft_cli(t_shell *sh);
t_shell	*ft_init_sh(char **envp);
void	ft_restore_cli(t_shell *sh, void *tree);

/* ft_env_manager.c */

char	**ft_get_my_envp(char **envp);
char	**ft_merge_env(char **env1, char **env2);

/* ft_copy_list.c */

t_list	**ft_copy_list(t_list **old);

#endif //MINISHELL_BONUS_H
