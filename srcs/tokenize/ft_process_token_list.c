#include "../../includes/minishell.h"

/**
 * @brief Determines if a string represents an expandable environment or status variable.
 * 
 * Checks if the string `s` begins with a valid environment variable name (alphabetic character
 * or underscore followed by an alphanumeric character) or with `$?`, indicating the exit status.
 * If so, the function returns `true`, indicating that the string can be expanded; otherwise, 
 * it returns `false`.
 * 
 * @param s Pointer to the string to check for expandability.
 * @return true if `s` represents an expandable variable, otherwise false.
 */
bool	ft_is_expandable(char *s)
{
	int	i;

	i = 0;
	if (ft_isalpha(s[i]) || s[i] == '?' || (s[i] == '_' && s[i + 1]
		&& ft_isalnum(s[i + 1])))
		return (true);
	return (false);
}

/**
 * @brief Removes leading and trailing quotes from a token's value.
 * 
 * This function iterates through a token's value string, 
 * removing both leading and trailing single or double quotes encountered.
 * 
 * @param token Pointer to the token structure containing the value to process.
 */
static void	ft_remove_quotes(t_token *token)
{
	char	quote;
	int		i;

	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$' && (token->value[i + 1] == SQUOTE || token->value[i + 1] == DQUOTE))
			ft_memmove(&token->value[i], &token->value[i + 1], ft_strlen(&token->value[i + 1]) + 1);
		if (token->value[i] == SQUOTE || token->value[i] == DQUOTE)
		{
			quote = token->value[i];
			ft_memmove(&token->value[i], &token->value[i + 1], ft_strlen(&token->value[i + 1]) + 1);
			while (token->value[i] && token->value[i] != quote)
				i++;
			if (token->value[i] == quote)
				ft_memmove(&token->value[i], &token->value[i + 1], ft_strlen(&token->value[i + 1]) + 1);
		}
		else
			i++;
	}
	return ;
}

/**
 * @brief Expands environment variables in a token's value.
 * 
 * This function iterates through each character in a token's value. If it encounters
 * single quotes, expansion doesn't occur. If it finds a `$` followed by
 * an expandable identifier, that is either between double quotes or none, it replaces it
 * with the corresponding environment variable value
 * The resulting expanded string is assigned to the token's value.
 * 
 * @param token Pointer to the token to be expanded.
 * @param my_envp Environment variable array used for expansion.
 */
static void	ft_expand_tokens(t_token *token, char **my_envp)
{
	char	*new_value;
	int		i;

	new_value = NULL;
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == SQUOTE)
			ft_handle_squotes(&new_value, token->value, &i);
		else if (token->value[i] == DQUOTE)
			ft_handle_dquotes(&new_value, token->value, &i, my_envp);
		else if (token->value[i] == '$' && ft_is_expandable(&token->value[i + 1]))
			ft_handle_expansion(&new_value, token->value, &i, my_envp);
		else if (token->value[i] && token->value[i] != DQUOTE
				&& token->value[i] != SQUOTE)
			new_value = ft_charjoin(new_value, token->value[i++]);
	}
	free(token->value);
	token->value = new_value;
}

/**
 * @brief Removes a specified node from a linked list.
 * 
 * Detaches the `current` node from the linked list, adjusting the pointers
 * of the previous node (`prev`) or the head of the list if `current` is the first node.
 * Frees the memory allocated to the `current` node using `ft_free_node`.
 * 
 * @param list Double pointer to the head of the list.
 * @param prev Pointer to the previous node, or NULL if `current` is the head.
 * @param current Pointer to the node to be removed.
 */
void	ft_remove_current_node(t_list **list, t_list *prev, t_list *current)
{
	t_list	*next;

	next = current->next;
	if (prev)
		prev->next = next;
	else
		*list = next;
	ft_free_content(current);
}

/**
 * @brief Processes a list of tokens to handle expansions and remove quotes.
 * 
 * Iterates over a list of tokens, expanding environment variables if needed,
 * removing quotes, and removing any token whose value becomes empty after expansion.
 * Updates the linked list to reflect any removed nodes.
 * 
 * @param list Double pointer to the head of the list of tokens.
 * @param my_envp Array of environment variables for token expansion.
 */
void	ft_process_token_list(t_list **list, char **my_envp)
{
	t_list	*current;
	//t_list	*prev;
	t_list	*next;
	t_token	*token;

	current = *list;
	//prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (token->expand)
			ft_expand_tokens(token, my_envp);
		if (token->state == IN_QUOTE)
			ft_remove_quotes(token);
		current = next;
	}
}
