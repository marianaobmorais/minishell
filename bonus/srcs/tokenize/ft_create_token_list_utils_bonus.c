#include "../../includes/minishell_bonus.h"

/**
 * @brief Determines if a command token represents an export command or an
 *        executable.
 * 
 * This function checks if the token string represents an export with an append
 * (`+=`), a simple export (`=`), or an executable command. It identifies
 * command type based on the initial characters of `s`.
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
 * @brief Determines the type of a token based on the given string.
 * 
 * The function inspects the provided string and identifies its type according
 * to the following logic:
 * - Special characters like `|`, `&`, `>`, `<`, `(`, `)` are mapped to their
 *   respective token types (e.g., `PIPE`, `AND`, `OUTFILE`, `PRTHESES`).
 * - The function checks for operators (like `>>`, `<<`, `||`, etc.) and
 *   matches them to the appropriate token type.
 * - For other cases, the function attempts to identify the token as a command
 *   by calling `ft_get_cmd_type`.
 * 
 * @param s The string representing a token.
 * @return The token type corresponding to the string.
 */
static t_type	ft_get_token_type(char *s)
{
	t_type	type;

	if (s[0] == '|' && s[1] == '|')
		return (OR);
	else if (s[0] == '|')
		return (PIPE);
	else if (s[0] == '>' && s[1] == '>')
		return (APPEND);
	else if (s[0] == '>')
		return (OUTFILE);
	else if (s[0] == '<' && s[1] == '<')
		return (HEREDOC);
	else if (s[0] == '<')
		return (INFILE);
	else if (s[0] == '&')
		return (AND);
	else if (s[0] == '(' || s[0] == ')')
		return (PRTHESES);
	else
	{
		type = ft_get_cmd_type(s);
		return (type);
	}
}

/**
 * @brief Checks if a token contains quoted text and returns its state.
 * 
 * This function scans through the token string to determine if it contains
 * single or double quotes. If so, it returns `IN_QUOTE`; otherwise, it returns
 * `GENERAL`.
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
 * This function analyzes the token string to determine if it contains
 * expandable variables within double quotes or outside of quotes.
 * Single-quoted variables are not expandable.
 * 
 * @param s The token string to analyze.
 * @return `true` if the token contains expandable variables; otherwise,
 *         `false`.
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

/**
 * @brief Adds a new token to the token list.
 * 
 * This function creates a new token structure based on the provided value, 
 * assigns its properties (e.g., type, state, wildcard status, expansion 
 * status), and appends it as a new node to the given token list. If memory 
 * allocation fails at any point, the function frees any partially allocated 
 * resources and invokes an error handler.
 * 
 * @param value Pointer to a dynamically allocated string representing the 
 *        token's value. The function frees the value after creating the token
 *        node.
 * @param token_list The token list to which the new token will be added.
 */
void	ft_add_to_token_list(char **value, t_list **token_list)
{
	t_token	*new_token;
	t_list	*new_node;

	if (!*value)
		return ;
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (ft_error_malloc("new_token"));
	new_token->value = ft_strdup(*value);
	if (!new_token->value)
		return (free(new_token), ft_error_malloc("new_token->value"));
	new_token->type = ft_get_token_type(*value);
	new_token->state = ft_get_token_state(*value);
	new_token->wildcard = false;
	new_token->expand = false;
	if (token_list && !ft_is_heredoc_target(token_list))
	{
		new_token->wildcard = ft_is_wildcard(*value);
		new_token->expand = ft_has_expandable_var(*value);
	}
	new_node = ft_lstnew((t_token *)new_token);
	if (!new_node)
		return (free(new_token->value), free(new_token),
			ft_error_malloc("new_node"));
	ft_lstadd_back(token_list, new_node);
	free(*value);
	*value = NULL;
}
