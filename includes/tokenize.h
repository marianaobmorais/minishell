#ifndef TOKENIZE_H
# define TOKENIZE_H

typedef enum s_type
{
	EXEC,
	PIPE
	//REDIR,
	//HEREDOC,
	//BUILTIN
}	t_type;

typedef enum s_state
{
	GENERAL,
	IN_SQUOTE,
	IN_DQUOTE
}	t_state;

typedef struct s_token
{
	char	*value;
	t_type	type;
	//t_state	state;
}	t_token;

typedef struct s_pipe
{
	int		type;
	char	*left;
	char	*right;
}	t_pipe;


#endif