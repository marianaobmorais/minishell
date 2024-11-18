#include "../../includes/minishell.h"

/**
 * @brief Checks if a character is a whitespace.
 * 
 * This function returns 1 if the character is a space (ASCII 32) or a whitespace character 
 * within the range of ASCII 9 to 13, otherwise returns 0.
 * 
 * @param c The character to check.
 * @return 1 if whitespace, otherwise 0.
 */
int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}
