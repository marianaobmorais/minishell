#ifndef STRUCTS_H
# define STRUCTS_H

# define EXEC 1
# define REDIR 2
# define PIPE 3
# define BACK 4

typedef struct s_pipe
{
	int		type;
	char	*left;
	char	*right;
}	t_pipe;

#endif