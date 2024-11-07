#include "../../includes/minishell.h"

/**
 * @brief Finds the next occurrence of a specified quote character in a string.
 * 
 * This function searches for the next occurrence of the specified quote character `c` 
 * in the string `s`, starting from index `i + 1`. If found, it returns the index of the 
 * quote; if not, it returns -1.
 * 
 * @param s The string to search.
 * @param i The starting index to begin searching from (excluding this position).
 * @param c The quote character to search for.
 * @return The index of the next occurrence of `c` or -1 if not found.
 */
int	ft_find_next_quote(char *s, int i, char c)
{
	i++;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}
