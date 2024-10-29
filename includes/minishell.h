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

# define PROMPT "bashinho: "
# define PROG_NAME "bashinho"
# define METACHARS "|&<>"

// srcs
int		ft_error_handler(void);
char	**ft_get_my_envp(char **envp);
char	**ft_split_expand(char *s, char c, char **my_envp);
char	*ft_charjoin(char *str, char c);

#endif