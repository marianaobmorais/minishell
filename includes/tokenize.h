#ifndef TOKENIZE_H
# define TOKENIZE_H

# define SPECIALCHARS "|<>&;()\\{}[^%."

typedef enum e_type
{
	EXEC,
	BUILTIN,
	PIPE,
	INFILE,
	OUTFILE,
	APPEND,
	HEREDOC
}	t_type;

typedef enum e_state
{
	GENERAL,
	IN_SQUOTE,
	IN_DQUOTE
}	t_state;

typedef struct s_token
{
	char	*value;
	t_type	type;
	t_state	state;
}	t_token;

typedef struct s_pipe
{
	int		type;
	char	*left;
	char	*right;
}	t_pipe;

int	ft_check_syntax(char *s);
int	ft_next_quote(char *s, int i, char c);
int	ft_isspace(int c);

#endif