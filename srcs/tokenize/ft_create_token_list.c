#include "../../includes/minishell.h"

/**
 * @brief Handles metacharacters and adds them as tokens to the token list.
 * 
 * Processes metacharacters found in the input string `s` by:
 * - Finalizing and adding any existing token value (`*value`) to the token
 *   list.
 * - Creating a new token for the current metacharacter(s).
 * - Detecting and handling compound metacharacters such as `>>`, `<<`, `>|`.
 * - Adding the newly formed token to the token list.
 * 
 * The function updates the token list and advances the index `i` if necessary
 * for compound metacharacters.
 * 
 * @param value Pointer to the current token value being processed. Reset after
 *              adding to the token list.
 * @param s Pointer to the current position in the input string.
 * @param i Current index in the input string.
 * @param list Pointer to the list where tokens will be added.
 * @return The updated index `i` after processing the metacharacter(s).
 */
static int	ft_handle_metachar(char **value, char *s, int i, t_list **token_list)
{
	if (value)
		ft_add_to_token_list(value, token_list); //update brief
	*value = ft_charjoin(*value, s[0]);
	if (s[0] == '>' && s[1] == '|') //might be redundant
	{
		*value = ft_charjoin(*value, s[1]);
		i++;
	}
	if ((s[0] == '>' || s[0] == '<') && ft_strchr(METACHARS, s[1]))
	{
		*value = ft_charjoin(*value, s[1]);
		i++;
	}
	ft_add_to_token_list(value, token_list);
	return (i);
}

/**
 * @brief Handles quoted sections in the input, adding them to the token string.
 * 
 * This function appends characters within a quoted section to `value`, including
 * the opening and closing quotes. It returns the updated index after the closing
 * quote.
 * 
 * @param value Pointer to the string being built for the current token.
 * @param s Pointer to the input string.
 * @param i The current index in the input string.
 * @param quote The quote character (`'` or `"`).
 * @return Updated index after processing the quoted section.
 */
static int	ft_handle_quotes(char **value, char *s, int i, char quote)
{
	*value = ft_charjoin(*value, s[i]);
	i++;
	while (s[i] != quote)
	{
		*value = ft_charjoin(*value, s[i]);
		i++;
	}
	*value = ft_charjoin(*value, s[i]);
	return (i);
}

/**
 * @brief Tokenizes the input string and populates the token list.
 * 
 * Processes the input string `s` by iterating through its characters and
 * identifying tokens based on metacharacters, quotes, and whitespace.
 * The function:
 * - Detects and handles metacharacters by delegating to `ft_handle_metachar`.
 * - Manages quoted substrings using `ft_handle_quotes`.
 * - Stops processing if a comment character (`#`) is encountered after
 *   whitespace.
 * - Concatenates unprocessed characters into a token value using `ft_charjoin`.
 * - Adds completed tokens to the token list when whitespace or a metacharacter
 *   is encountered.
 * 
 * @param s The input string to process and tokenize.
 * @param token_list A pointer to the list where tokens will be added.
 */
static void	ft_process_tokens(char *s, t_list **token_list)
{
	char	*value;
	int		i;

	i = 0;
	value = NULL;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
		{
			if (ft_strchr(METACHARS, s[i]))
				i = ft_handle_metachar(&value, &s[i], i, token_list); 
			else if (s[i] == SQUOTE || s[i] == DQUOTE)
				i = ft_handle_quotes(&value, s, i, s[i]);
			else if (s[i] == '#' && ft_isspace(s[i - 1]))
				break ;
			else
				value = ft_charjoin(value, s[i]);
		}
		if (ft_isspace(s[i]) && value)
			ft_add_to_token_list(&value, token_list);
		i++;
	}
	ft_add_to_token_list(&value, token_list);
}

/**
 * @brief Validates and adjusts the types of tokens related to `EXPORT` and
 *        `EXPORT_AP`.
 * 
 * Iterates through the token list to ensure that `EXPORT` and `EXPORT_AP`
 * tokens are properly contextualized based on their position and the preceding
 * token.
 * - If an `EXPORT` or `EXPORT_AP` token appears after the first position and
 *   doesn't follow a valid token type (`PIPE`, another `EXPORT`, `EXPORT_AP`),
 *   its type is changed to `EXEC`.
 * - This ensures proper token classification and avoids misinterpretation
 *   during execution.
 * 
 * @param list A pointer to the head of the token list to validate and adjust.
 */

void	ft_validate_export_tokens(t_list **list)
{
	t_list	*current;
	t_token	*token;
	t_list	*prev;
	int	pos;

	pos = 0;
	current = *list;
	prev = NULL;
	while (current)
	{
		token = (t_token *)current->content;
		if ((token->type == EXPORT || token->type == EXPORT_AP) && pos > 0
				&& !(((t_token *)prev->content)->type == PIPE
				|| ((t_token *)prev->content)->type == EXPORT
				|| ((t_token *)prev->content)->type == EXPORT_AP))
			token->type = EXEC;
		pos++;
		prev = current;
		current = current->next;
	}
}

/**
 * @brief Creates and processes token list from the input string.
 * 
 * This function allocates memory for a token list, processes the input string to 
 * identify tokens, populates the list and applies necessary processing steps such as
 * handling `EXPORT` tokens. It frees the input string after processing.
 * 
 * @param s The input string to tokenize.
 * @return A pointer to the list of tokens, or NULL if allocation fails.
 */
t_list	**ft_create_token_list(char *s)
{
	t_list	**token_list;

	token_list = (t_list **)malloc(sizeof(t_list **));
	if (!token_list)
		return (NULL); // ft_error_handler(); 1 //malloc failed
	*token_list = NULL;
	ft_process_tokens(s, token_list);
	ft_validate_export_tokens(token_list);
	return (free(s), token_list);
}
