/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:02:26 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/22 17:04:25 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_BONUS_H
# define TOKENIZE_BONUS_H

//chars
# define SPECIALCHARS "{}^!"
# define INVALIDCHARS ";\\`~["
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
	NODE,
	REDIR_OUT
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
	bool	wildcard;
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
	void	*next;
}	t_redir;

typedef struct s_exec
{
	int		type;
	t_list	**args;
}	t_exec;

/* ft_find_next_quote_bonus.c */

int		ft_find_next_quote(char *s, int i, char c);

/* ft_charjoin_bonus.c */

char	*ft_charjoin(char *str, char c);

/* ft_isspace_bonus.c */

int		ft_isspace(int c);

/* ft_validate_syntax_bonus.c */

bool	ft_validate_syntax(char *s);
void	ft_error_syntax(char *message, char c);

/* ft_validate_syntax_utils_bonus.c */

bool	ft_validate_logic_operator(char *s, int i);
bool	ft_is_comment(char c, int *i);

/* ft_ft_validate_parentheses_bonus.c */

bool	ft_validate_parentheses(char *s);
bool	ft_count_parentheses(char *s);

/* ft_create_token_list_bonus.c */

t_list	**ft_create_token_list(char *s);

/* ft_create_token_list_utils_bonus.c */

void	ft_add_to_token_list(char **value, t_list **token_list);
bool	ft_has_expandable_var(char *s);

/* ft_create_token_list_utils2_bonus.c */

bool	ft_is_heredoc_target(t_list **list);
bool	ft_is_wildcard(char *s);
void	ft_validate_export_tokens(t_list **list);
void	ft_validate_token_expansion(t_token *new_token, char *value);

/* ft_process_input_bonus.c */

void	*ft_process_input(char *input);

/* ft_process_token_list_bonus.c */

void	ft_process_token_list(t_list **token_list, char **envp);
void	ft_remove_current_node(t_list **list, t_list *prev, t_list *curr);

/* ft_process_token_list_utils_bonus.c */

void	ft_handle_dquotes(char **new_value, char *value, int *i, \
		char **envp);
void	ft_handle_squotes(char **new_value, char *value, int *i);
void	ft_handle_expansion(char **new_value, char *value, int *i, \
		char **envp);

/* ft_process_token_list_utils2_bonus.c */

void	ft_expand_tokens(t_token *token, char **envp);
void	ft_remove_quotes(t_token *tkn);
bool	ft_is_expandable(char *s);

/* ft_build_root_bonus.c */

void	*ft_build_root(t_list **list, t_type node_type);

/* ft_build_tree_bonus.c */

void	*ft_build_tree(t_list **token_list, t_node **parent_node);
void	ft_skip_export_tokens(t_list **list);
bool	ft_validate_skip(t_list **list);

/* ft_build_branch_bonus.c */

void	*ft_build_branch(t_list **list, t_exec *exec, t_node *sub_root);

/* ft_built_branch_utils_bonus.c */

t_exec	*ft_create_exec_node(t_token *token, t_list **list);
bool	ft_find_next_redir(t_list **list);

/* ft_built_branch_utils2_bonus.c */

t_node	*ft_create_subroot_node(t_list **list);
t_redir	*ft_init_redir(t_token *token, t_list **list);

/* ft_free_tree_bonus.c */

void	ft_free_tree(void *root);

/* ft_is_token_type_bonus.c */

bool	ft_is_token_type(t_token *token, t_type type);
bool	ft_is_node_type(t_node *node, t_type type);

#endif //TOKENIZE_BONUS_H