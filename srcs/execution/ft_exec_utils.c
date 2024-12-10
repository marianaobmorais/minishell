#include "../../includes/minishell.h"

/**
 * @brief Converts a list of tokens to an array of strings.
 *
 * Iterates through a linked list of tokens, converting each token's value
 * to a string and storing it in a newly allocated array. Handles memory
 * allocation and error checking.
 *
 * @param args A double pointer to the list of tokens.
 *
 * @return A newly allocated array of strings, or NULL on error.
 */
char	**tokentostring(t_list **args)
{
	char	**new_args;
	t_list	*curr_list;
	int		size;
	int		i;

	curr_list = *args;
	size = ft_lstsize(curr_list);
	i = 0;
	new_args = (char **)malloc((size + 1) * sizeof(char *));
	if (!new_args)
		ft_error_malloc("new_args");
	while (curr_list)
	{
		new_args[i] = ft_strdup(((t_token *)(curr_list)->content)->value);
		i++;
		curr_list = (curr_list)->next;	
	}
	new_args[i] = NULL;
	return (new_args);
}

/**
 * @brief Counts the number of words in an array of strings separated by a
 *        delimiter.
 *
 * Iterates through each string in the array to count words separated by
 * the specified delimiter. A word is defined as a sequence of characters
 * that are not equal to the delimiter.
 *
 * @param args The array of strings to be processed.
 * @param c The delimiter character.
 *
 * @return The total number of words found in all strings.
 */
static int	ft_count_words(char **args, char c)
{
	int		size;
	int		i;
	bool	in_word;

	size = 0;
	while (*args)
	{
		in_word = false;
		i = 0;
		while ((*args)[i] != '\0')
		{
			if ((*args)[i] != c && !in_word)
			{
				in_word = true;
				size++;
			}
			else if ((*args)[i] == c)
				in_word = false;
			i++;
		}
		args++;
	}
	return (size);
}

/**
 * @brief Splits a string into words and adds them to an array of strings.
 *
 * Splits the given string `arg` by spaces and appends each word to the
 * `new_args` array at the position indicated by `*y`. Updates `*y` as
 * words are added. Handles memory allocation and cleanup for temporary
 * data.
 *
 * @param arg The string to split into words.
 * @param new_args The array where words are stored.
 * @param y A pointer to the current index in `new_args`, updated as words
 *          are added.
 *
 * @return `new_args` if successful, or NULL if an error occurs.
 */
static char	**ft_split_and_copy(char *arg, char **new_args, int *y)
{
	//need to double check function
	char	**temp;
	int		z;

	temp = ft_split(arg, ' ');
	if (!temp)
		return (ft_error_malloc("temp"), NULL);
	z = 0;
	while (temp[z])
	{
		new_args[*y] = ft_strdup(temp[z]);
		if (!new_args[*y])
			return (ft_free_vector(temp), NULL);
		(*y)++;
		z++;
	}
	ft_free_vector(temp);
	return (new_args);
}

/**
 * @brief Splits an array of strings into words and stores them in a new array.
 *
 * Processes each string in the `args` array, splitting them by spaces
 * and appending the resulting words to a new dynamically allocated array.
 * Frees the original `args` array after processing.
 *
 * @param args The array of strings to split.
 *
 * @return A new array containing all words, or NULL if a memory allocation
 *         error occurs.
 */
char	**ft_split_argv(char **args)
{
	//need to double check function
	int		i;
	int		y;
	char	**new_args;

	i = 0;
	y = 0;
	new_args = (char **)malloc((ft_count_words(args, ' ') + 1) * sizeof(char *));
	if (!new_args)
		return (ft_error_malloc("new_args"), NULL);
	while (args[i])
	{
		if (!ft_split_and_copy(args[i], new_args, &y))
			return (ft_free_vector(new_args), NULL);
		i++;
	}
	new_args[y] = NULL;
	ft_free_vector(args);
	return (new_args);
}
