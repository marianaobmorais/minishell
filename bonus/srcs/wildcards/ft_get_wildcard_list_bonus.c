#include "../../includes/minishell_bonus.h"

/**
 * @brief Determines if the given string contains a prefix before the first '*'.
 *
 * Checks whether the string starts with a character other than '*' to identify
 * if a prefix exists. If the first character is not '*', a prefix is assumed.
 *
 * @param s The string to be checked.
 * @return `true` if a prefix exists, otherwise `false`.
 */
static bool	ft_find_prefix(char *s)
{
	int		i;

	i = 0;
	if (!s)
		return (false);
	if (s[i] != '*')
		return (true);
	else
		return (false);
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
bool	ft_find_middle(char *s, bool prefix)
{
	int		i;

	i = 0;
	if (!s)
		return (false);
	if (prefix)
		while (s[i] != '*')
			i++;
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
 * @brief Determines if the given string contains a suffix after the last '*'.
 *
 * Searches for the last '*' in the string and checks if there are characters 
 * following it. If such characters exist, a suffix is assumed.
 *
 * @param s The string to be checked.
 * @return `true` if a suffix exists, otherwise `false`.
 */
static bool	ft_find_sufix(char *s)
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

/**
 * @brief Generates a list of directory entries matching a wildcard pattern.
 *
 * Creates a list of directory entries from the current directory and filters 
 * them based on the provided wildcard pattern. The filtering process matches 
 * prefixes, middle segments, and suffixes according to the pattern structure. 
 * The resulting list is sorted alphabetically.
 *
 * @param s The wildcard pattern to be matched.
 * @return A pointer to a list of tokens representing the matching entries, 
 * or `NULL` if an error occurred.
 */
t_list	**ft_get_wildcard_list(char *s)
{
	t_list	**wild_list;

	wild_list = ft_copy_entry_list();
	if (ft_find_prefix(s) && wild_list && *wild_list)
		ft_match_prefix(wild_list, s);
	if (ft_find_middle(s , ft_find_prefix(s)) && wild_list && *wild_list)
		ft_match_middle(wild_list, s, ft_find_prefix(s));
	if (ft_find_sufix(s) && wild_list && *wild_list)
		ft_match_sufix(wild_list, s);
	if (*wild_list)
		wild_list = ft_set_alphabetical_order(wild_list);
	return (wild_list);
}
