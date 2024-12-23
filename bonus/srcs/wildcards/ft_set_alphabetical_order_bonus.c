#include "../../includes/minishell_bonus.h"

/**
 * @brief Compares two strings in a case-insensitive manner.
 *
 * This function performs a character-by-character comparison of two strings, 
 * converting both characters to lowercase during the comparison. It returns 
 * the difference between the first mismatched characters or 0 if the strings 
 * are equal.
 *
 * @param str1 The first string to compare.
 * @param str2 The second string to compare.
 * @return An integer greater than, equal to, or less than 0 if `str1` is 
 *         lexicographically greater than, equal to, or less than `str2`.
 */
static int	ft_strcmp_lower(const char *str1, const char *str2)
{
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while (*s1 != '\0' || *s2 != '\0')
	{
		if (ft_tolower(*s1) != ft_tolower(*s2))
			return (ft_tolower(*s1) - ft_tolower(*s2));
		++s1;
		++s2;
	}
	return (0);
}

/**
 * @brief Sorts an array of strings in alphabetical order (case-insensitive).
 *
 * This function sorts the given array of strings using the bubble sort 
 * algorithm and performs case-insensitive comparisons to determine order.
 *
 * @param str The array of strings to be sorted.
 * @param n The number of elements in the array.
 */
static void	ft_sort_str_tab_(char **str, int n)
{
	int		k;
	int		j;
	char	*swap;

	k = 0;
	while (k < n - 1)
	{
		j = 0;
		while (j < n - k - 1)
		{
			if (ft_strcmp_lower(str[j], str[j + 1])
				> ft_strcmp_lower(str[j + 1], str[j]))
			{
				swap = str[j];
				str[j] = str[j + 1];
				str[j + 1] = swap;
			}
			j++;
		}
		k++;
	}
}

/**
 * @brief Sorts a list of tokens alphabetically and returns the new list.
 *
 * This function converts the tokens in the given list to an array of strings, 
 * sorts the strings alphabetically in a case-insensitive manner, and then 
 * recreates a token list from the sorted strings. The old token list and 
 * string array are freed before returning the new list.
 *
 * @param old A pointer to the original list of tokens to sort.
 * @return A pointer to the new list of tokens sorted alphabetically, or `NULL` 
 *         if memory allocation fails.
 */
t_list	**ft_set_alphabetical_order(t_list **old)
{
	t_list	**new;
	char	**tmp;
	int		size;
	int		i;

	new = (t_list **)malloc(sizeof(t_list *));
	if (!new)
		return (ft_error_malloc("new"), NULL);
	*new = NULL;
	tmp = tokentostring(old);
	size = 0;
	while (tmp[size])
		size++;
	ft_sort_str_tab_(tmp, size);
	i = 0;
	while (tmp[i])
	{
		ft_add_to_token_list(&tmp[i], new);
		i++;
	}
	ft_free_list(*old);
	free(old);
	ft_free_vector(tmp);
	return (new);
}
