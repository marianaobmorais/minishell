#include "../../includes/minishell_bonus.h"

/**
 * @brief Extracts the prefix of a wildcard pattern before the first `*`.
 * 
 * This function iterates through the string `s` and constructs a new string 
 * containing all characters up to (but not including) the first occurrence 
 * of the `*` character.
 * 
 * @param s The wildcard pattern to extract the prefix from.
 * @return A dynamically allocated string containing the prefix. Returns 
 *         `NULL` if memory allocation fails or `s` does not contain `*`.
 */
static char	*ft_get_prefix(char *s)
{
	char	*prefix;
	int		i;

	i = 0;
	prefix = NULL;
	if (!s)
		return (prefix);
	while (s[i] != '*')
	{
		prefix = ft_charjoin(prefix, s[i]);
		i++;
	}
	return (prefix);
}

/**
 * @brief Extracts the suffix of a wildcard pattern after the last `*`.
 * 
 * This function locates the last occurrence of `*` in the string `s` and 
 * constructs a new string containing all characters after it.
 * 
 * @param s The wildcard pattern to extract the suffix from.
 * @return A dynamically allocated string containing the suffix. Returns 
 *         `NULL` if memory allocation fails or `s` does not contain `*`.
 */
static char	*ft_get_sufix(char *s)
{
	char	*sufix;
	char	*tmp;
	int		i;

	i = 0;
	sufix = NULL;
	if (!s)
		return (sufix);
	tmp = ft_strrchr(s, '*');
	while (tmp[i + 1])
	{
		sufix = ft_charjoin(sufix, tmp[i + 1]);
		i++;
	}
	return (sufix);
}

/**
 * @brief Filters a list of tokens to match entries with a specific prefix.
 *
 * Iterates through the provided list and removes nodes whose values do not 
 * start with the specified prefix. The prefix is extracted from the given 
 * wildcard pattern. Frees the prefix memory after filtering.
 *
 * @param list A pointer to the list of tokens to be filtered.
 * @param s The wildcard pattern containing the prefix to match.
 */
void	ft_match_prefix(t_list **list, char *s)
{
	t_list *current;
	t_list	*prev;
	t_list	*next;
	t_token	*token;
	char	*prefix;

	prefix = ft_get_prefix(s);
	if (!prefix)
		return ;
	current = *list;
	prev = NULL;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (ft_strncmp(token->value, prefix, ft_strlen(prefix)))
			ft_remove_current_node(list, prev, current);
		else
			prev = current;
		current = next;
	}
	free(prefix);
}

/**
 * @brief Extracts the suffix substring from a token's value.
 *
 * Retrieves the substring at the end of the token's value that matches the 
 * length of the specified suffix. The function handles memory allocation 
 * for the resulting substring and returns it.
 *
 * @param token The token containing the value from which the substring is
 *        extracted.
 * @param sufix The suffix pattern to match.
 * @return A dynamically allocated substring matching the suffix, or `NULL` if
 *         allocation fails.
 */
static char	*ft_get_substring(t_token *token, char *sufix)
{
	char	*substring;
	size_t	sufix_len;
	size_t	value_len;

	sufix_len = ft_strlen(sufix);
	value_len = ft_strlen(token->value);
	substring = NULL;
	substring = ft_substr(token->value, value_len - sufix_len, sufix_len);
	if (!substring)
		ft_error_malloc("substring");
	return (substring);
}

/**
 * @brief Filters a list of tokens to match entries with a specific suffix.
 *
 * Iterates through the provided list and removes nodes whose values do not 
 * end with the specified suffix. The suffix is extracted from the given 
 * wildcard pattern. Frees the suffix and any intermediate substrings after
 * filtering.
 *
 * @param list A pointer to the list of tokens to be filtered.
 * @param s The wildcard pattern containing the suffix to match.
 */
void	ft_match_sufix(t_list **list, char *s)
{
	t_list *current;
	t_list	*prev;
	t_list	*next;
	char	*sufix;
	char	*substring;

	sufix = ft_get_sufix(s);
	if (!sufix)
		return ;
	current = *list;
	prev = NULL;
	while (current)
	{
		next = current->next;
		substring = ft_get_substring((t_token *)current->content, sufix);
		if (!substring)
			return (free(sufix));
		if (ft_strncmp_(substring, sufix, ft_strlen(sufix)))
			ft_remove_current_node(list, prev, current);
		else
			prev = current;
		current = next;
		free(substring);
	}
	free(sufix);
}
