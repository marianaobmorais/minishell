#include "../../includes/minishell.h"

/**
 * @brief Determines if a string represents an expandable environment or status
 *        variable.
 * 
 * Checks if the string `s` begins with a valid environment variable name
 * (alphabetic character or underscore followed by an alphanumeric character)
 * or with `$?`, indicating the exit status. If so, the function returns `true`
 * indicating that the string can be expanded; otherwise, it returns `false`.
 * 
 * @param s Pointer to the string to check for expandability.
 * @return true if `s` represents an expandable variable, otherwise false.
 */
bool	ft_is_expandable(char *s)
{
	int	i;

	i = 0;
	if (ft_isalpha(s[i]) || s[i] == '?' || s[i] == '*'
		|| (s[i] == '_' && s[i + 1] && ft_isalnum(s[i + 1])))
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
		if (token->value[i] == '$' &&
			(token->value[i + 1] == SQUOTE || token->value[i + 1] == DQUOTE))
			ft_memmove(&token->value[i], &token->value[i + 1],
				ft_strlen(&token->value[i + 1]) + 1);
		if (token->value[i] == SQUOTE || token->value[i] == DQUOTE)
		{
			quote = token->value[i];
			ft_memmove(&token->value[i], &token->value[i + 1],
				ft_strlen(&token->value[i + 1]) + 1);
			while (token->value[i] && token->value[i] != quote)
				i++;
			if (token->value[i] == quote)
				ft_memmove(&token->value[i], &token->value[i + 1],
					ft_strlen(&token->value[i + 1]) + 1);
		}
		else
			i++;
	}
	return ;
}

/**
 * @brief Expands environment variables in a token's value.
 * 
 * This function iterates through each character in a token's value. If it
 * encounters single quotes, expansion doesn't occur. If it finds a `$`
 * followed by an expandable identifier, that is either between double quotes
 * or none, it replaces it with the corresponding environment variable value
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
		else if (token->value[i] == '$'
			&& ft_is_expandable(&token->value[i + 1]))
			ft_handle_expansion(&new_value, token->value, &i, my_envp);
		else if (token->value[i] && token->value[i] != DQUOTE
				&& token->value[i] != SQUOTE)
			new_value = ft_charjoin(new_value, token->value[i++]);
	}
	free(token->value);
	token->value = new_value;
}

/**
 * @brief Processes the token list for expansion and quote removal.
 * 
 * Iterates over a list of tokens, applying transformations to each token:
 * - Expands tokens that require environment variable expansion (`expand` flag
 *   set).
 * - Removes quotes from tokens that are marked as being inside quotes
 *   (`state == IN_QUOTE`).
 * 
 * Ensures each token is fully prepared for further parsing or execution. This
 * function does not modify the structure of the token list itself.
 * 
 * @param list Double pointer to the head of the token list.
 * @param my_envp Array of environment variables used for token expansion.
 */
void	ft_process_token_list(t_list **list, char **my_envp)
{
	t_list	*current; //need to receive both global and local envp //update brief
	t_list	*prev;
	t_token	*token;
	t_list	**wild_list;

	current = *list;
	prev = NULL;
	while (current)
	{
		token = (t_token *)current->content;
		if (token->expand)
			ft_expand_tokens(token, my_envp);
		if (token->state == IN_QUOTE)
			ft_remove_quotes(token);
		if (token->wildcard)
		{
			wild_list = ft_get_wildcard_list(token->value); //allocated memory for wild_list
			if (*wild_list)
			{
				ft_update_token_list(current, prev, list, wild_list);
				current = *wild_list;
			}
			if (wild_list) //should I break it here in case !wild_list?
				free(wild_list);
		}
		prev = current;
		current = current->next;
	}
}
