#include "../../includes/minishell.h"

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
static char *ft_get_sufix(char *s)
{
	char	*sufix;
	char	*tmp;
	int		i;

	i = 0;
	sufix = NULL;
	tmp = ft_strrchr(s, '*');
	while (tmp[i + 1])
	{
		sufix = ft_charjoin(sufix, tmp[i + 1]);
		i++;
	}
	return (sufix);
}

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
 * 
 * @return An integer less than, equal to, or greater than zero if `str1` is
 *         found to be less than, equal to, or greater than `str2`,
 *         respectively. Returns `0` if either `str1` or `str2` is `NULL`.
 */
static int	ft_strncmp_(const char *str1, const char *str2, size_t len)
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
 * @brief Validates if a directory entry matches a wildcard pattern.
 * 
 * This function checks if the given `entry_name` starts with the prefix and 
 * ends with the suffix extracted from the wildcard pattern `s`. It uses 
 * helper functions to extract the prefix and suffix and performs comparisons 
 * to validate the match.
 * 
 * @param s The wildcard pattern containing `*` for matching.
 * @param entry_name The directory entry name to validate.
 * 
 * @return `true` if the `entry_name` matches the wildcard pattern, otherwise
 *         `false`.
 */
bool	ft_validate_entry(char *s, char *entry_name)
{
	char	*prefix;
	char	*sufix;
	char	*substring;
	bool	result;

	result = false;
	prefix = ft_get_prefix(s);
	sufix = ft_get_sufix(s);
	substring = ft_substr(entry_name, ft_strlen(entry_name) - ft_strlen(sufix),
		ft_strlen(sufix));
	if (ft_strncmp(entry_name, prefix, ft_strlen(prefix)) == 0
		&& ft_strncmp_(substring, sufix, ft_strlen(sufix)) == 0)
		result = true;
	free(substring);
	free(prefix);
	free(sufix);
	return (result);
}
