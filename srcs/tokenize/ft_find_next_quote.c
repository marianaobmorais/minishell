#include "../../includes/minishell.h"

/**
 * @brief Finds the next matching quote in a string, with special handling for
 *        unsupported expansions.
 *
 * This function searches for the next occurrence of a specified quote
 * character `quote` in the string `s`, starting from index `i + 1`. If it
 * encounters an unsupported `$` expansion within double quotes (like `$`
 * followed by certain characters defined in `NON_EXPANDABLE`), it prints an
 * error message and returns `-1`. If no matching quote is found, it also
 * prints an error message about open quotes and returns `-1`.
 *
 * @param s The string to search through.
 * @param i The starting index after which to search for the quote.
 * @param quote The quote character to search for (single or double quote).
 * @return The index of the matching quote, or -1 if an error occurs.
 */
int	ft_find_next_quote(char *s, int i, char quote)
{
	i++;
	while (s[i] != '\0')
	{
		if (s[i] == quote)
			return (i);
		i++;
	}
	return (-1);
}
