/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_wildcard_list_utils_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:45:33 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/24 16:18:43 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/**
 * @brief Extracts the prefix from a wildcard pattern before the first wildcard.
 *
 * Parses the input wildcard pattern `s` to extract the substring before the
 * first wildcard (`*`). Updates the pointer `s` to point to the position of the
 * first wildcard in the string.
 *
 * @param s A double pointer to the wildcard pattern string. The pointer is
 *        updated to point at the first wildcard in the pattern.
 * @return The extracted prefix string, or `NULL` if no prefix exists or on
 *         memory allocation failure.
 */
static char	*ft_get_prefix(char **s)
{
	char	*prefix;
	char	*ptr;

	prefix = NULL;
	if (!s || !*s)
		return (prefix);
	ptr = *s;
	while (*ptr != '*')
	{
		prefix = ft_charjoin(prefix, *ptr);
		ptr++;
	}
	*s = ptr;
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
 * @brief Matches a prefix from a wildcard pattern with the start of an entry
 *        name.
 *
 * Extracts the prefix from the wildcard pattern `s` and compares it with the
 * start of the entry name `entry_name`. Advances both pointers to reflect
 * the successful matching of the prefix, if applicable.
 *
 * @param s A double pointer to the wildcard pattern string, updated to point
 *        beyond the extracted prefix.
 * @param entry_name A double pointer to the entry name string, updated to
 *        reflect the consumed prefix on successful match.
 * @return `true` if the prefix matches the start of the entry name, otherwise
 *         `false`.
 */
bool	ft_match_prefix(char **s, char **entry_name)
{
	char	*prefix;
	bool	result;

	result = true;
	prefix = ft_get_prefix(s);
	if (!prefix)
		ft_error_malloc("prefix");
	if (ft_strncmp_(*entry_name, prefix, ft_strlen(prefix)))
			result = false;
	else
		*entry_name = *entry_name + ft_strlen(prefix);
	if (prefix)
		free(prefix);
	return (result);
}

/**
 * @brief Matches a suffix from a wildcard pattern with the end of an entry name
 *
 * Extracts the suffix from the wildcard pattern `s` and compares it with the
 * end of the entry name `entry_name`. The function dynamically allocates memory
 * for the suffix and the substring being compared, ensuring to free them
 * before returning.
 *
 * @param s A double pointer to the wildcard pattern string.
 * @param entry_name A double pointer to the entry name string.
 * @return `true` if the suffix matches the end of the entry name, otherwise
 *         `false`.
 */
bool	ft_match_sufix(char **s, char **entry_name)
{
	char	*sufix;
	char	*substring;
	bool	result;
	size_t	sufix_len;

	result = true;
	sufix = ft_get_sufix(*s);
	if (!sufix)
		return (ft_error_malloc("sufix"), result);
	sufix_len = ft_strlen(sufix);
	substring = ft_substr(*entry_name, ft_strlen(*entry_name) - sufix_len,
		sufix_len);
	if (!substring)
		ft_error_malloc("substring");
	if (ft_strncmp_(substring, sufix, ft_strlen(sufix)))
		result = false;
	if (substring)
		free(substring);
	if (sufix)
		free(sufix);
	return (result);
}

/**
 * @brief Determines if the given string contains a suffix after the last '*'.
 *
 * Searches for the last '*' in the string and checks if there are characters 
 * following it. If such characters exist, a suffix is assumed.
 *
 * @param s The string to be checked.
 * @return `true` if a suffix exists, otherwise `false`.
 */
bool	ft_find_sufix(char *s)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!s)
		return (false);
	tmp = ft_strrchr(s, '*');
	if (tmp[i + 1])
		return (true);
	return (false);
}
