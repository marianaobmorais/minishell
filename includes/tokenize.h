#ifndef TOKENIZE_H
# define TOKENIZE_H

# define SPECIALCHARS "{}[^!"
# define INVALIDCHARS ";()\\`*~"
# define METACHARS "|<>&"
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
	EXPORT_AP,
	AND,
	OR,
	PRTHESIS,
	ROOT
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

// typedef struct s_pipe
// {
// 	int		type;
// 	char	*left; // aponta para redir ou exec
// 	char	*right; // aponta para pipe
// }	t_pipe;

typedef struct s_node
{
	int		type;
	void	*left;
	void	*right;
	t_node	*parent_node; //insert to tree
}	t_node;

typedef struct s_redir
{
	int		type;
	t_list	**target;
	int		mode;
	int		fd;
	void	*next; // aponta para o node de exec ou um redir, nunca pipe
}	t_redir;

typedef struct s_exec
{
	int		type;
	char	*pathname;//useful for debug when printing
	t_list	**args;
}	t_exec;

//ft_find_next_quote.c
int		ft_find_next_quote(char *s, int i, char c);

//ft_charjoin.c
char	*ft_charjoin(char *str, char c);

//ft_isspace.c
int	ft_isspace(int c);

//ft_check_syntax.c
int		ft_validate_syntax(char *s);
int		ft_isspace(int c);

//ft_create_token_list.c
t_list	**ft_create_token_list(char *s);
//ft_create_token_list_utils.c
void	ft_add_to_token_list(char **value, t_list **token_list);

//ft_process_input.c
void	*ft_process_input(char *input, char **my_envp);

//ft_process_token_list.c // move to execution
void	ft_process_token_list(t_list **token_list, char** my_envp); // move to execution
bool	ft_is_expandable(char *s);

//ft_process_token_list_utils.c
void	ft_handle_dquotes(char **new_value, char *value, int *i, char **my_envp);
void	ft_handle_squotes(char **new_value, char *value, int *i);
void	ft_handle_expansion(char **new_value, char *value, int *i, char **my_envp);
char	*ft_get_exit_code(int *i);
char	*ft_expand_env(char *s, int *i, char **my_envp);

//ft_build_root.c
void	*ft_build_root(t_list **list, t_type type);
void	ft_skip_export_tokens(t_list **list);

//ft_build_tree.c
void	*ft_build_tree(t_list **token_list);
bool	ft_validate_next_token(t_list **list);

//ft_build_branch.c
void	*ft_build_branch(t_list **list, t_exec *exec);
//ft_built_branch_utils.c
t_list	**ft_get_args(t_list **list);
bool	ft_find_next_redir(t_list **list);
bool	ft_find_next_exec(t_list **list);
char	**ft_add_to_vector(char **vector, char *new_str); // move to execution

//ft_free_tree.c
void	ft_free_tree(void *root); //update this

#endif //TOKENIZE_H