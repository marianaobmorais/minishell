/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_token_list_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:16:33 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/29 15:53:34 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

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
 *        adding to the token list.
 * @param s Pointer to the current position in the input string.
 * @param i Current index in the input string.
 * @param list Pointer to the list where tokens will be added.
 * @return The updated index `i` after processing the metacharacter(s).
 */
static int	ft_handle_metachar(char **value, char *s, int i, t_list **list)
{
	if (value)
		ft_add_to_token_list(value, list);
	*value = ft_charjoin(*value, s[0]);
	if (s[0] == '>' && s[1] == '|')
	{
		*value = ft_charjoin(*value, s[1]);
		i++;
	}
	else if ((ft_strchr(METACHARS, s[0])) && (s[0] == s[1]))
	{
		*value = ft_charjoin(*value, s[1]);
		i++;
	}
	ft_add_to_token_list(value, list);
	return (i);
}

/**
 * @brief Handles quoted sections in the input, adding them to the token string
 * 
 * This function appends characters within a quoted section to `value`,
 * including the opening and closing quotes. It returns the updated index after
 * the closing quote.
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
 * - Detects and handles metacharacters and parentheses by delegating to
 *   `ft_handle_metachar`.
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
			if (ft_strchr(METACHARS, s[i]) || ft_strchr(PRTHESESCHARS, s[i]))
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
 * @brief Validates execution tokens to ensure proper placement and syntax.
 * 
 * Checks the token list for invalid scenarios involving execution tokens
 * (e.g., commands, subshells). Specifically, the function:
 * - Tracks context (`right`) to identify whether an EXEC token follows invalid
 *   tokens (e.g., `)`).
 * - Ensures execution tokens do not appear directly after certain invalid
 *   sequences.
 * - Skips over redirection tokens as they do not affect EXEC token validation.
 * 
 * Errors are reported using `ft_stderror` with appropriate messages, and the
 * function sets an error status before returning `false` when invalid syntax
 * is detected.
 * 
 * @param list Pointer to the token list to be validated.
 * @return `true` if the tokens are valid, `false` otherwise.
 */
static bool	ft_validate_exec_tokens(t_list **list)
{
	t_list	*current;
	t_token	*token;
	bool	right;

	current = *list;
	right = false;
	while (current)
	{
		token = (t_token *)current->content;
		if (token->value[0] == '(' || ft_is_token_type(token, NODE))
			right = false;
		else if (token->value[0] == ')')
			right = true;
		else if (ft_is_token_type(token, REDIR))
			current = current->next;
		else if (right == true && ft_is_token_type(token, EXEC))
		{
			ft_stderror(FALSE, UNEXPECTED_TOKEN_S, token->value);
			ft_exit_status(2, TRUE, FALSE);
			return (false);
		}
		current = current->next;
	}
	return (true);
}

/**
 * @brief Creates a linked list of tokens from the input string.
 * 
 * Allocates memory for a list and processes the input string to tokenize it.
 * - Splits the input string into individual tokens based on predefined
 *   delimiters.
 * - Processes the tokens and adds them to the list.
 * - Validates export tokens to ensure syntax compliance.
 * - Validates executable tokens for correctness.
 * 
 * If any validation or memory allocation fails, the function returns `NULL`.
 * 
 * @param s The input string to tokenize.
 * @return A pointer to the head of the token list, or `NULL` on failure.
 */
t_list	**ft_create_token_list(char *s)
{
	t_list	**token_list;

	token_list = (t_list **)malloc(sizeof(t_list *));
	if (!token_list)
		return (ft_error_malloc("token_list"), NULL);
	*token_list = NULL;
	ft_process_tokens(s, token_list);
	ft_validate_export_tokens(token_list);
	if (!ft_validate_exec_tokens(token_list))
	{
		ft_free_list(*token_list);
		free(token_list);
		return (NULL);
	}
	return (token_list);
}
