#include "../../includes/minishell.h"

/**
 * @brief Validates the correct usage of logical operators in a string.
 * 
 * This function checks whether the current character in a string represents a 
 * logical operator (`&` or `|`) and ensures it is not preceded by a space or 
 * followed by invalid syntax. It also validates against other special characters 
 * or cases like `.` followed by a space or null terminator.
 * 
 * @param s The input string being validated.
 * @param i The current index in the string to validate.
 * @return `true` if the logical operator usage is valid; `false` otherwise.
 */
bool	ft_validate_logic_operator(char *s , int i)
{
	if (((s[i] == '&' || s[i] == '|') && ft_isspace(s[i - 1]))
			|| ft_strchr(SPECIALCHARS, s[i]) || (s[i] == '.'
			&& (ft_isspace(s[i + 1]) || s[i + 1] == '\0')))
		return(false);
	else
		return (true);
}

/**
 * @brief Checks if the given character indicates the start of a comment.
 * 
 * This function determines if a character is the start of a comment (e.g., `#`).
 * If a comment is detected, the index pointer is incremented to skip the character,
 * and the function returns `true`. Otherwise, it returns `false`.
 * 
 * @param c The character to check.
 * @param i Pointer to the current index in the input string, updated if a comment is detected.
 * @return `true` if the character indicates the start of a comment, otherwise `false`.
 */
bool	ft_is_comment(char c, int *i)
{
	if (c == '#')
	{
		(*i)++;
		return (true);
	}
	return (false);
}
