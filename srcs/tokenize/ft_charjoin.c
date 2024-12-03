#include "../../includes/minishell.h"

/**
 * @brief Concatenates a character to the end of a string and returns the new
 *        string.
 *
 * This function allocates memory for a new string, copies the original string
 * into it, appends the specified character to the end, and then returns the
 * newly created string. The original string is freed after the operation. The
 * new string is null-terminated.
 *
 * @param str The original string to which the character will be appended.
 * @param c The character to append to the string.
 * @return A new string with the character appended, or NULL if memory
 *         allocation fails.
 */
char	*ft_charjoin(char *str, char c)
{
	size_t	len;
	size_t	i;
	char	*res;

	len = ft_strlen(str);
	res = (char *)malloc(sizeof(char) * (len + 2));
	if (res == NULL)
		return (NULL); //error_hanlder; 1//malloc failed
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		++i;
	}
	res[i] = c;
	res[++i] = '\0';
	if (str)
		free(str);
	return (res);
}