#ifndef TOKENIZE_H
# define TOKENIZE_H

# define SPECIALCHARS "&;()\\{}[^%.`*"
# define METACHARS "|<>"
# define ISSPACE " \t\n\v\f\r"
# define SQUOTE 39
# define DQUOTE 34

typedef enum e_type
{
	PIPE,
	APPEND,
	OUTFILE,
	HEREDOC,
	INFILE,
	EXEC,
	EXPORT,
	EXPORT_AP
}	t_type;

typedef enum e_state
{
	GENERAL,
	IN_QUOTE
}	t_state;

typedef struct s_token
{
	char	*value;
	t_type	type;
	t_state	state;
	bool	expand;
}	t_token;

typedef struct s_pipe
{
	int		type;
	char	*left;
	char	*right;
}	t_pipe;

//ft_check_syntax.c
int		ft_check_syntax(char *s);
int		ft_isspace(int c);

int		ft_next_quote(char *s, int i, char c);
void	ft_process_input(char *input, char **my_envp);
char	*ft_charjoin(char *str, char c);
t_list	**ft_create_token_list(char *s);
void	ft_add_to_token_list(char **value, t_list **token_list);
void	ft_process_token_list(t_list **token_list, char** my_envp);

//ft_process_token_list_utils.c
void	ft_handle_dquotes(char **new_value, char *value, int *i, char **my_envp);
void	ft_handle_squotes(char **new_value, char *value, int *i);
void	ft_handle_expansion(char **new_value, char *value, int *i, char **my_envp);
char	*ft_get_exit_code(int *i);
char	*ft_expand_env(char *s, int *i, char **my_envp);


#endif //TOKENIZE_H