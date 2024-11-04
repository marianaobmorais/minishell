#ifndef TOKENIZE_H
# define TOKENIZE_H

# define SPECIALCHARS "&;()\\{}[^%.`*"
# define METACHARS "|<>"
# define ISSPACE " \t\n\v\f\r"
# define SQUOTE 39
# define DQUOTE 34

typedef enum e_type
{
	//BUILTIN,
	PIPE,
	APPEND,
	OUTFILE,
	HEREDOC,
	INFILE,
	EXPORT, // delete of keep?
	EXEC
}	t_type;

typedef enum e_state
{
	IN_SQUOTE,
	IN_DQUOTE,
	GENERAL
}	t_state;

typedef struct s_token
{
	char	*value;
	t_type	type;
	t_state	state;
	bool	expand;//
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
t_list	**ft_create_token_list(char *s);
t_type	ft_get_type(char *s);
t_state	ft_get_state(char *s);
void	ft_parse_token_list(t_list **token_list, char** my_envp);

#endif