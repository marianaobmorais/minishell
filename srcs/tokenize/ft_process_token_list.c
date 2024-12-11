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
 * @param tkn Pointer to the token structure containing the value to process.
 */
static void	ft_remove_quotes(t_token *tkn)
{
	char	quote;
	int		i;

	i = 0;
	while (tkn->value[i])
	{
		if (tkn->value[i] == '$'
			&& (tkn->value[i + 1] == SQUOTE || tkn->value[i + 1] == DQUOTE))
			ft_memmove(&tkn->value[i], &tkn->value[i + 1],
				ft_strlen(&tkn->value[i + 1]) + 1);
		if (tkn->value[i] == SQUOTE || tkn->value[i] == DQUOTE)
		{
			quote = tkn->value[i];
			ft_memmove(&tkn->value[i], &tkn->value[i + 1],
				ft_strlen(&tkn->value[i + 1]) + 1);
			while (tkn->value[i] && tkn->value[i] != quote)
				i++;
			if (tkn->value[i] == quote)
				ft_memmove(&tkn->value[i], &tkn->value[i + 1],
					ft_strlen(&tkn->value[i + 1]) + 1);
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
 * @param envp Environment variable array used for expansion.
 */
static void	ft_expand_tokens(t_token *token, char **envp)
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
			ft_handle_dquotes(&new_value, token->value, &i, envp);
		else if (token->value[i] == '$'
			&& ft_is_expandable(&token->value[i + 1]))
			ft_handle_expansion(&new_value, token->value, &i, envp);
		else if (token->value[i] && token->value[i] != DQUOTE
			&& token->value[i] != SQUOTE)
			new_value = ft_charjoin(new_value, token->value[i++]);
	}
	free(token->value);
	token->value = new_value;
}

/**
 * @brief Processes a wildcard token and replaces it with expanded matches.
 * 
 * This function handles the expansion of wildcard patterns in a token list. If
 * a wildcard pattern in the current token matches multiple file paths or
 * items, the function replaces the current token with a list of tokens
 * representing the matches. If no matches are found, the token remains
 * unchanged.
 * 
 * @param current A double pointer to the current position in the token list.
 *        This pointer may be updated to point to the new list of tokens if a
 *        wildcard is expanded.
 * @param prev A pointer to the previous node in the token list, used for
 *        relinking after replacement.
 * @param head A double pointer to the head of the token list. This is updated
 *        if the head is replaced.
 */
static void	ft_handle_wildcard(t_list **current, t_list *prev, t_list **head)
{
	t_list	**wild_list;
	t_token	*token;

	token = (t_token *)(*current)->content;
	wild_list = ft_get_wildcard_list(token->value);
	if (wild_list && *wild_list)
	{
		ft_update_token_list(*current, prev, head, wild_list);
		*current = *wild_list;
	}
	if (wild_list)
		free(wild_list);
}

/**
 * @brief Removes a specified node from a linked list.
 * 
 * Detaches the `current` node from the linked list, adjusting the pointers
 * of the previous node (`prev`) or the head of the list if `current` is the
 * first node. Frees the memory allocated to the `current` node using
 * `ft_free_node`.
 * 
 * @param list Double pointer to the head of the list.
 * @param prev Pointer to the previous node, or NULL if `current` is the head.
 * @param curr Pointer to the node to be removed.
 */
static void	ft_remove_current_node(t_list **list, t_list *prev, t_list *curr)
{
	t_list	*next;

	next = NULL;
	next = curr->next;
	if (prev)
		prev->next = next;
	else
		*list = next;
	ft_free_content(curr);
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
 * @param envp Array of environment variables used for token expansion.
 */
void	ft_process_token_list(t_list **list, char **envp)
{
	t_list	*current; //update brief and refactor
	t_list	*prev;
	t_list	*next;
	t_token	*token;

	current = *list;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (token->expand)
			ft_expand_tokens(token, envp);
		if (token->state == IN_QUOTE)
			ft_remove_quotes(token);
		if (!*token->value && token->expand && !token->state)
		{
			ft_remove_current_node(list, prev, current);
			current = next;
			continue ;
		}
		if (token->wildcard)
		{
			ft_handle_wildcard(&current, prev, list);
			prev = NULL;
			next = current->next;
		}
		else
			prev = current;
		current = next;
	}
	ft_free_vector(envp);
}
