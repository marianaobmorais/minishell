#include "../../includes/minishell.h"

/**
 * @brief Determines if a command token represents an export command or an executable.
 * 
 * This function checks if the token string represents an export with an append (`+=`), 
 * a simple export (`=`), or an executable command. It identifies command type based on 
 * the initial characters of `s`.
 * 
 * @param s The token string to check.
 * @return The command type: EXPORT_AP, EXPORT, or EXEC.
 */
static t_type	ft_get_cmd_type(char *s)
{
	int	i;

	i = 0;
	if (ft_isalpha(s[i]) || s[i] == '_')
	{
		while (ft_isalnum(s[i]) || s[i] == '_')
			i++;
		if (s[i] == '+' && s[i + 1] == '=')
			return (EXPORT_AP);
		else if (s[i] == '=')
			return (EXPORT);
	}
	return (EXEC);
}

/**
 * @brief Determines the token type based on metacharacters or command type.
 * 
 * This function identifies if the token is a metacharacter (like `|`, `>`, `<`, `>>`, `<<`) or 
 * a command type by checking for specific characters in `s`. If not a metacharacter, it 
 * uses `ft_get_cmd_type` to further classify the token.
 * 
 * @param s The token string to evaluate.
 * @return The type of token, e.g., PIPE, APPEND, OUTFILE, HEREDOC, INFILE, EXPORT, etc.
 */
static t_type	ft_get_token_type(char *s)
{
	t_type	type;

	if (s[0] == '|')
	{
		if (s[1] == '|')
			return (OR);
		return (PIPE);
	}
	else if (s[0] == '>')
	{
		if (s[1] == '>')
			return (APPEND);
		return (OUTFILE);
	}
	else if (s[0] == '<')
	{
		if (s[1] == '<')
			return (HEREDOC);
		return (INFILE);
	}
	else if (s[0] == '&')
		return (AND);
	else
	{
		type = ft_get_cmd_type(s);
		return (type);
	}
}

/**
 * @brief Checks if a token contains quoted text and returns its state.
 * 
 * This function scans through the token string to determine if it contains single or 
 * double quotes. If so, it returns `IN_QUOTE`; otherwise, it returns `GENERAL`.
 * 
 * @param s The token string to examine.
 * @return The state of the token: IN_QUOTE or GENERAL.
 */
static t_state	ft_get_token_state(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == SQUOTE || s[i] == DQUOTE)
			return (IN_QUOTE);
		i++;
	}
	return (GENERAL);
}

/**
 * @brief Checks if a token contains variables that are expandable.
 * 
 * This function analyzes the token string to determine if it contains expandable 
 * variables within double quotes or outside of quotes. Single-quoted variables are not 
 * expandable.
 * 
 * @param s The token string to analyze.
 * @return `true` if the token contains expandable variables; otherwise, `false`.
 */
static bool	ft_has_expandable_var(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == SQUOTE)
			i = ft_find_next_quote(s, i, SQUOTE) + 1;
		else if (s[i] == DQUOTE)
		{
			i++;
			while (s[i] && s[i] != DQUOTE)
			{
				if (s[i] == '$' && ft_is_expandable(&s[i + 1])) 
					return (true);
				i++;
			}
		}
		else if (s[i] == '$' && ft_is_expandable(&s[i + 1]))
			return (true);
		else if (s[i])
			i++;
	}
	return (false);
}

bool	ft_is_heredoc_target(t_list **list)
{
	t_list	*last;
	t_token	*token;

	last = ft_lstlast(*list);
	if (!last || !last->content)
		return (false);
	token = (t_token *)last->content;
	if (token->type == HEREDOC)
		return (true);
	return (false);
}

/**
 * @brief Adds a new token to the token list after initializing it.
 * 
 * This function creates a new token from `*value`, determining its type, state, and 
 * whether it contains expandable variables. It then creates a new list node for this 
 * token and adds it to the end of `token_list`.
 * 
 * @param value Pointer to the current token string being built.
 * @param token_list Pointer to the list of tokens.
 */
void	ft_add_to_token_list(char **value, t_list **token_list)
{
	t_token	*new_token;
	t_list	*new_node;

	if (*value)
	{
		new_token = (t_token *)malloc(sizeof(t_token));
		if (!new_token)
			return ; // ft_error_handler();
		new_token->value = ft_strdup(*value);
		if (!new_token->value)
			return (free(new_token));
		new_token->type = ft_get_token_type(*value);
		new_token->state = ft_get_token_state(*value);
		if (!ft_is_heredoc_target(token_list))
			new_token->expand = ft_has_expandable_var(*value);
		new_node = ft_lstnew((t_token *)new_token);
		if (!new_node)
			return (free(new_token->value), free(new_token));
		ft_lstadd_back(token_list, new_node);
		free(*value);
		*value = NULL;
	}
}
