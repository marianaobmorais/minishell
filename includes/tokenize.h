#ifndef TOKENIZE_H
# define TOKENIZE_H

//chars
# define SPECIALCHARS "{}[^!"
# define INVALIDCHARS "&;()\\`*~"
# define METACHARS "|<>"
# define ISSPACE " \t\n\v\f\r"
# define SQUOTE 39
# define DQUOTE 34

//messages
# define UNEXPECTED_TOKEN "syntax error near unexpected token `%c'"
# define UNEXPECTED_TOKEN_S "syntax error near unexpected token `%s'"
# define OPEN_QUOTE "open quotes are not supported"

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
	char	*left; // aponta para redir ou exec
	char	*right; // aponta para pipe
	//struct s_node	*parent_node; //not necessary in mandatory
}	t_pipe;

typedef struct s_redir
{
	int		type;
	t_list	**target; // pointer to next token, file or limiter
	int		mode;
	void	*next; // aponta para o node de exec ou um redir, nunca pipe
}	t_redir;

typedef struct s_exec
{
	int		type;
	char	*pathname; // precisa desse?
	t_list	**args;
}	t_exec;

typedef struct s_envp
{
	char	**envp_global;
	char	**envp_local;
}	t_envp;

//ft_find_next_quote.c
int		ft_find_next_quote(char *s, int i, char c);

//ft_charjoin.c
char	*ft_charjoin(char *str, char c);

//ft_isspace.c
int	ft_isspace(int c);

//ft_validate_syntax.c
bool	ft_validate_syntax(char *s);
//ft_validate_syntax_utils.c
bool	ft_is_comment(char c, int *i);

//ft_isspace.c
int		ft_isspace(int c);

//ft_create_token_list.c
t_list	**ft_create_token_list(char *s);
//ft_create_token_list_utils.c
void	ft_add_to_token_list(char **value, t_list **token_list);

//ft_process_input.c
void	*ft_process_input(char *input);

//ft_process_token_list.c
void	ft_process_token_list(t_list **token_list, char** my_envp);
bool	ft_is_expandable(char *s);

//ft_process_token_list_utils.c
void	ft_handle_dquotes(char **new_value, char *value, int *i, char **my_envp);
void	ft_handle_squotes(char **new_value, char *value, int *i);
void	ft_handle_expansion(char **new_value, char *value, int *i, char **my_envp);
char	*ft_get_exit_code(int *i);
char	*ft_expand_env(char *s, int *i, char **my_envp);

//ft_build_tree.c
void	*ft_build_tree(t_list **token_list);

//ft_build_branch.c
void	*ft_build_branch(t_list **list, t_exec *exec);
//ft_built_branch_utils.c
char	**ft_add_to_vector(char **vector, char *new_str);
char	**ft_get_args(t_list **list);
bool	ft_find_next_redir(t_list **list);
bool	ft_find_next_exec(t_list **list);

//ft_free_tree.c
void	ft_free_tree(void *root);

#endif //TOKENIZE_H