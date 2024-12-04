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
	new_args = (char **) malloc((size + 1) * sizeof(char *));
	if (!new_args)
		ft_error_handler();
	while (curr_list)
	{
		new_args[i] = ft_strdup(((t_token *)(curr_list)->content)->value);
		curr_list = (curr_list)->next;
		i++;
	}
	new_args[i] = NULL;
	return (new_args);
}

/**
 * @brief Counts the number of words in a string separated by a delimiter.
 *
 * Iterates through the string to count words separated by the specified
 * delimiter. A word is defined as a sequence of characters not equal to
 * the delimiter.
 *
 * @param s The input string to be processed.
 * @param c The delimiter character.
 *
 * @return The number of words found in the string.
 */
static int	ft_count_words(char **args, char c)
{
	bool	in_word;
	int		count;
	int		i;
	int		size;

	size = 0;
	while (*args)
	{
		in_word = false;
		count = 0;
		i = 0;
		while ((*args)[i] != '\0')
		{
			if ((*args)[i] != c && in_word == false)
			{
				in_word = true;
				count++;
			}
			if ((*args)[i] == c)
				in_word = false;
			i++;
		}
		size += count;
		args++;
	}
	ft_stderror(FALSE, "size ->%d", size);
	return (size);
}

char	**ft_split_argv(char **args)
{
	int		i;
	int		z;
	int		y;
	char	**new_args;
	char	**temp;

	i = 0;
	y = 0;
	new_args = (char **) malloc((ft_count_words(args, ' ') * sizeof(char *)) + 1);
	if (!new_args)
		return (NULL); //ft malloc
	while (args[i])
	{
		temp = ft_split(args[i], ' ');
		z = 0;
		while (temp[z])
		{
			new_args[y++] = ft_strdup(temp[z++]);

		}
		ft_stderror(FALSE, "tem-> %s", temp[z - 1]);
		ft_free_vector(temp);
		i++;
	}
	ft_stderror(FALSE, "y-> %d", y);
	//new_args[y] = NULL;
	return (ft_free_vector(args), new_args);
}
