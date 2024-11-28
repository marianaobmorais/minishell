#ifndef TOKENIZE_H
# define TOKENIZE_H

//chars
# define SPECIALCHARS "{}[^!"
# define INVALIDCHARS ";\\`*~"
# define PRTHESESCHARS "()"
# define METACHARS "|<>&"
# define ISSPACE " \t\n\v\f\r"
# define SQUOTE 39
# define DQUOTE 34

//messages
# define UNEXPECTED_TOKEN "syntax error near unexpected token `%c'"
# define UNEXPECTED_TOKEN_S "syntax error near unexpected token `%s'"
# define OPEN_QUOTE "open quotes are not supported"
# define OPEN_PRTHESES "open parentheses are not supported"

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
	PRTHESES,
	ROOT,
	SUB_ROOT,
	REDIR,
	NODE
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

typedef struct s_node
{
	int				type;
	void			*left;
	void			*right;
	struct s_node	*parent_node;
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
int		ft_isspace(int c);

//ft_validate_syntax.c
bool	ft_validate_syntax(char *s);
void	ft_error_syntax(char *message, char c);
//ft_validate_syntax_utils.c
bool	ft_validate_logic_operator(char *s, int i);
bool	ft_is_comment(char c, int *i);

//ft_ft_validate_parentheses.c
bool	ft_validate_parentheses(char *s);
bool	ft_count_parentheses(char *s);
//ft_ft_validate_parentheses_utils.c
bool	ft_handle_paretheses(char c, char *last, bool *left, bool *right);
bool	ft_validate_left_context(char *s, int i, bool *left);
bool	ft_handle_chars(char *s, int *i, bool *left, bool *right);

//ft_create_token_list.c
t_list	**ft_create_token_list(char *s);
//ft_create_token_list_utils.c
void	ft_add_to_token_list(char **value, t_list **token_list);

//ft_process_input.c
void	*ft_process_input(char *input);

//ft_process_token_list.c
void	ft_process_token_list(t_list **token_list, \
		char **my_envp);
bool	ft_is_expandable(char *s);

//ft_process_token_list_utils.c
void	ft_handle_dquotes(char **new_value, char *value, int *i, \
		char **my_envp);
void	ft_handle_squotes(char **new_value, char *value, int *i);
void	ft_handle_expansion(char **new_value, char *value, int *i, \
		char **my_envp);
char	*ft_get_exit_code(int *i);
char	*ft_expand_env(char *s, int *i, char **my_envp);

//ft_build_root.c
void	*ft_build_root(t_list **list, t_type type);
void	ft_skip_export_tokens(t_list **list);

//ft_build_tree.c
void	*ft_build_tree(t_list **token_list, t_node **parent_node);

//ft_build_branch.c
void	*ft_build_branch(t_list **list, t_exec *exec, t_node *sub_root);
//ft_built_branch_utils.c
t_list	**ft_get_args(t_list **list);
bool	ft_find_next_redir(t_list **list);
t_list	**ft_create_sub_list(t_list **list);
t_node	*ft_create_subroot_node(t_list **list);
t_exec	*ft_create_exec_node(t_token *token, t_list **list);
void	ft_assign_redir_mode(t_redir **redir);
t_redir	*ft_init_redir(t_token *token, t_list **list);

//ft_free_tree.c
void	ft_free_tree(void *root);
void	ft_free_node(void *node);
void	ft_free_redir(t_redir *redir);
void	ft_free_exec(t_exec *exec);

//ft_is_token_type.c
bool	ft_is_token_type(t_token *token, t_type type);

//delete later
void	ft_print_list(t_list **token_list);

#endif //TOKENIZE_H