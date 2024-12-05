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
char	*ft_get_prefix(char *s)
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
char	*ft_get_sufix(char *s)
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

char	*ft_get_middle(char *s)
{
	char	*middle;
	int		i;

	i = 0;
	middle = NULL;
	while (s[i] == '*')
		i++;
	if (s[i] != '\0')
	{
		while (s[i] && s[i] != '*')
		{
			middle = ft_charjoin(middle, s[i]);
			i++;
		}
		if (s[i] != '*')
			return (NULL);
	}
	return (middle);
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
 * @brief Searches for a string in another string.
 * 
 * This function seachers the first `len` characters of `big` for `little` 
 * while ensuring that both strings are non-NULL. The comparison is case-
 * sensitive and follows the behavior of the standard `strnstr` function.
 * 
 * @param big The string to be searched.
 * @param little The string to be found in `big`.
 * @param len The maximum number of characters to search.
 * 
 * @return If `little` is found, it returns pointer to `big` where `little`
 *         starts, else, it returns `NULL` if `little` is `NULL` or `little`
 *         is not found.
 */
char	*ft_strnstr_(const char *big, const char *little, size_t len)
{
	char	*b;
	size_t	l_len;

	b = (char *)big;
	if (!little || !*little)
		return (b);
	l_len = ft_strlen(little);
	while (*b != '\0' && len > 0)
	{
		if (ft_strncmp(b, little, l_len) == 0 && l_len <= len)
			return (b);
		++b;
		--len;
	}
	return (NULL);
}
