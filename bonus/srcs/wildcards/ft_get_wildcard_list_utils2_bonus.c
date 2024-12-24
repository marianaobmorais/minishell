/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_wildcard_list_utils2_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:45:36 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/24 16:12:55 by marianamora      ###   ########.fr       */
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
 * @brief Checks if a wildcard pattern has a middle segment between '*'
 *        characters.
 *
 * Evaluates if there is a non-empty segment between '*' characters in the given 
 * string. If a prefix exists, it skips the prefix before analyzing the middle.
 *
 * @param s The string to be checked.
 * @param prefix A boolean indicating if the string has a prefix.
 * @return `true` if a middle segment exists, otherwise `false`.
 */
bool	ft_find_middle(char *s)
{
	int		i;

	i = 0;
	if (!s)
		return (false);
	while (s[i] == '*')
		i++;
	if (s[i])
	{
		while (s[i] && s[i] != '*')
			i++;
		if (s[i] != '*')
			return (false);
		else
			return (true);
	}
	return (false);
}

/**
 * @brief Extracts the middle substring from a wildcard pattern.
 *
 * Parses the input wildcard pattern `s` to extract the next substring between
 * wildcards (`*`). Advances the pointer `s` to the next position after the
 * extracted substring or to the end of the string if no further wildcard is
 * present.
 *
 * @param s A double pointer to the wildcard pattern string. The pointer is
 *        updated to point beyond the extracted middle substring.
 * @return The extracted middle substring, or `NULL` if no substring exists or
 *         on memory allocation failure.
 */
static char	*ft_get_middle(char **s)
{
	char	*middle;
	char	*ptr;

	middle = NULL;
	if (!*s)
		return (middle);
	ptr = *s;
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
 * @brief Matches middle substrings in a wildcard pattern with an entry name.
 *
 * Validates if the middle substrings extracted from the wildcard pattern `s`
 * are present in sequence within the entry name. Updates the wildcard pattern
 * pointer `s` and the entry name pointer `entry_name` as the matching process
 * advances.
 *
 * @param s A double pointer to the wildcard pattern string, updated as
 *        substrings are processed.
 * @param entry_name A double pointer to the entry name string, updated as
 *        substrings are matched.
 * @return `true` if all middle substrings match the entry name in order,
 *         otherwise `false`.
 */

bool	ft_match_middle(char **s, char **entry_name)
{
	char	*middle;
	bool	result;

	result = false;
	if (!s || !*s || !entry_name || !*entry_name)
		return (true);
	middle = ft_get_middle(s);
	if (!middle)
		ft_error_malloc("middle");
	if (ft_strnstr_(entry_name, middle, ft_strlen(*entry_name)))
		result = true;
	if (ft_find_middle(*s))
		result = ft_match_middle(s, entry_name);
	if (middle)
		free(middle);
	return (result);
}

