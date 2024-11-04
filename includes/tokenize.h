#ifndef TOKENIZE_H
# define TOKENIZE_H

# define SPECIALCHARS "&;()\\{}[^%.`*"
# define METACHARS "|<>"
# define ISSPACE " \t\n\v\f\r"

typedef enum e_type
{
	EXEC,
	//BUILTIN,
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
	IN_DQUOTE,
	EXPORT //
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

int		ft_check_syntax(char *s);
int		ft_next_quote(char *s, int i, char c);
int		ft_isspace(int c);
void	ft_process_input(char *input, char **my_envp);
char	*ft_charjoin(char *str, char c);

#endif