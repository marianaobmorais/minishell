/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:03:06 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 19:03:08 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	EXPORT_AP,
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

/* ft_find_next_quote.c */

int		ft_find_next_quote(char *s, int i, char c);

/* ft_charjoin.c */

char	*ft_charjoin(char *str, char c);

/* ft_isspace.c */

int		ft_isspace(int c);

/* ft_validate_syntax.c */

bool	ft_validate_syntax(char *s);

/* ft_validate_syntax_utils.c */

bool	ft_validate_logic_operator(char *s, int i);
bool	ft_is_comment(char c, int *i);

/* ft_create_token_list.c */

t_list	**ft_create_token_list(char *s);

/* ft_create_token_list_utils.c */

void	ft_add_to_token_list(char **value, t_list **token_list);

/* ft_create_token_list_utils2.c */

bool	ft_is_heredoc_target(t_list **list);

/* ft_process_input.c */

void	*ft_process_input(char *input);

/* ft_process_token_list.c */

void	ft_process_token_list(t_list **token_list, char **envp);
bool	ft_is_expandable(char *s);

/* ft_process_token_list_utils.c */

void	ft_handle_dquotes(char **new_value, char *value, int *i, \
		char **envp);
void	ft_handle_squotes(char **new_value, char *value, int *i);
void	ft_handle_expansion(char **new_value, char *value, int *i, \
		char **envp);

/* ft_get_wildcard_list.c */

void	ft_update_token_list(t_list *curr, t_list *prev, t_list **head, \
		t_list **w_list);

/* ft_build_tree.c */

void	*ft_build_tree(t_list **token_list);

/* ft_build_branch.c */

void	*ft_build_branch(t_list **list, t_exec *exec);

/* ft_built_branch_utils.c */

t_exec	*ft_create_exec_node(t_token *token, t_list **list);

/* ft_free_tree.c */

void	ft_free_tree(void *root);

/* ft_is_token_type.c */

bool	ft_is_token_type(t_token *token, t_type type);
bool	ft_is_node_type(t_node *node, t_type type);

#endif //TOKENIZE_H