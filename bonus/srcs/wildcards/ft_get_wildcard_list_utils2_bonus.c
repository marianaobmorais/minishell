/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_wildcard_list_utils2_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:45:36 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/23 16:48:35 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Compares two strings up to `len` characters, with additional null
 *        checks.
 * 
 * This function compares the first `len` characters of `str1` and `str2` 
 * while ensuring that both strings are non-NULL. The comparison is case-
 * sensitive and follows the behavior of the standard `strncmp` function.
 * 
 * @param str1 The first string to compare.
 * @param str2 The second string to compare.
 * @param len The maximum number of characters to compare.
 * @return An integer less than, equal to, or greater than zero if `str1` is
 *         found to be less than, equal to, or greater than `str2`,
 *         respectively. Returns `0` if either `str1` or `str2` is `NULL`.
 */
int	ft_strncmp_(const char *str1, const char *str2, size_t len)
{
	unsigned char	*s1;
	unsigned char	*s2;

	if (!str1 || !str2)
		return (0);
	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while ((*s1 != '\0' || *s2 != '\0') && (len > 0))
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		++s1;
		++s2;
		--len;
	}
	return (0);
}

/**
 * @brief Finds the first occurrence of a substring within a limited portion of
 *        a string.
 *
 * Searches for the first occurrence of the substring `little` within the first 
 * `len` characters of the string `big`. Updates the pointer `*big` to the
 * position just after the found substring, or to the end of the string if not
 * found.
 *
 * @param big Pointer to the string to search, updated to the position after the
 *        match.
 * @param little The substring to search for.
 * @param len The maximum number of characters to search.
 * @return A pointer to the start of the found substring within `big`, or `NULL`
 *         if not found.
 */
char	*ft_strnstr_(char **big, const char *little, size_t len)
{
	char	*ptr;
	size_t	l_len;

	if (!little || !*little)
		return (*big);
	ptr = *big;
	l_len = ft_strlen(little);
	while (*ptr && len > 0)
	{
		if (ft_strncmp(ptr, little, l_len) == 0 && l_len <= len)
		{
			*big = ptr + l_len;
			return (ptr);
		}
		ptr++;
		len--;
	}
	*big = ptr;
	return (NULL);
}

/**
 * @brief Extracts the middle substring from a wildcard pattern.
 *
 * Processes the given string pointer `*s` to extract the substring between
 * wildcard `*` characters. Updates the pointer `*s` to the position just after
 * the extracted middle substring. If no valid middle substring exists, frees
 * any allocated memory and returns `NULL`.
 *
 * @param s A pointer to the string pattern being processed, updated to the next
 *        position.
 * @param prefix Indicates whether the prefix of the pattern has been processed.
 * @return A dynamically allocated middle substring, or `NULL` if no middle
 *         substring is found.
 */
char	*ft_get_middle(char **s, bool prefix)
{
	char	*middle;
	char	*ptr;

	middle = NULL;
	if (!*s)
		return (middle);
	ptr = *s;
	if (prefix)
		while (*ptr != '*')
			ptr++;
	while (*ptr == '*')
		ptr++;
	while (*ptr && *ptr != '*')
	{
		middle = ft_charjoin(middle, *ptr);
		ptr++;
	}
	*s = ptr;
	if (*ptr != '*')
	{
		if (middle)
			free(middle);
		return (NULL);
	}
	return (middle);
}

/**
 * @brief Validates if the middle substrings in a wildcard pattern match the
 *        entry name.
 *
 * Recursively checks if all middle substrings in the wildcard pattern `s` exist
 * within `entry_name` in order of appearance. The function handles prefix
 * conditions and updates the pattern pointer during processing.
 *
 * @param s The wildcard pattern containing middle substrings.
 * @param entry_name The name of the entry to be checked against the pattern.
 * @param prefix Indicates if the prefix of the pattern has been processed.
 * @return `true` if all middle substrings match the entry name in order,
 *         otherwise `false`.
 */
bool	ft_handle_middle(char *s, char *entry_name, bool prefix)
{
	char	*middle;
	bool	result;

	if (!s || !entry_name)
		return (false);
	result = false;
	middle = ft_get_middle(&s, prefix);
	if (!middle)
		return (false);
	if (ft_strnstr_(&entry_name, middle, ft_strlen(entry_name)) != NULL)
		result = true;
	if (ft_find_middle(s, false))
		result = ft_handle_middle(s, entry_name, false);
	free(middle);
	return (result);
}

/**
 * @brief Filters a list of tokens to match entries containing middle substrings
 *
 * Iterates through the token list and removes nodes whose values do not contain 
 * all middle substrings from the wildcard pattern `s` in order of appearance.
 * Handles prefix conditions during matching.
 *
 * @param list A pointer to the list of tokens to be filtered.
 * @param s The wildcard pattern containing the middle substrings to match.
 * @param prefix Indicates whether the prefix of the pattern has been processed.
 */
void	ft_match_middle(t_list **list, char *s, bool prefix)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;
	t_token	*token;

	prev = NULL;
	current = *list;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->content;
		if (!ft_handle_middle(s, token->value, prefix))
			ft_remove_current_node(list, prev, current);
		else
			prev = current;
		current = next;
	}
}
