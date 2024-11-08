#include "../../includes/minishell.h"

bool	ft_is_expandable(char *s)
{
	int	i;

	i = 0;
	if (ft_isalpha(s[i]) || s[i] == '?' || (s[i] == '_' && s[i + 1]
		&& ft_isalnum(s[i + 2])))
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
		else if (token->value[i] && token->value[i] != DQUOTE && token->value[i] != SQUOTE)
			new_value = ft_charjoin(new_value, token->value[i++]);
	}
	free(token->value);
	token->value = new_value;
}

/**
 * @brief Processes a list of tokens to handle expansions and remove quotes.
 * 
 * This function iterates through each token in a list. If a token is marked as expandable,
 * it expands any eligible variables in its value. It also removes quotes from tokens that are 
 * flagged as having quoted segments.
 * 
 * @param list Pointer to the list of tokens to process.
 * @param my_envp Environment variable array used for expansion.
 */
void	ft_process_token_list(t_list **list, char **my_envp)
{
	t_list	*current;
	t_token	*token;

	current = *list;
	while (current)
	{
		token = (t_token *)current->content;
		if (token->expand == true)
			ft_expand_tokens(token, my_envp);
		if (token->state == IN_QUOTE)
			ft_remove_quotes(token);
		current = current->next;
	}
}
