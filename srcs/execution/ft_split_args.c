#include "../../includes/minishell.h"

static int	ft_count_words(char *s, char c)
{
	bool	in_word;
	int		count;
	int		i;

	in_word = false;
	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && in_word == false)
		{
			in_word = true;
			count++;
		}
		if (s[i] == 39 || s[i] == 34)
		{
			i = ft_find_next_quote(s, i, s[i]);
			if (i == -1)
				return (-1);
		}
		if (s[i] == c)
			in_word = false;
		i++;
	}
	return (count);
}

// int	ft_offset_str(char *s, char c)
// {
// 	int		len;

// 	len = 0;
// 	while (s[len] != '\0' && s[len] != c)
// 	{
// 		if (s[len] == 39) 
// 			len = ft_find_next_quote(s, len, 39);
// 		else if (s[len] == 34)
// 			len = ft_find_next_quote(s, len, 34);
// 		if (s[len] != c)
// 			len++;
// 	}
// 	return (len);
// }

// char	*ft_copy_word(char *s, int len)
// {
// 	char	*word;
// 	int		i;

// 	word = NULL;
// 	i = 0;
// 	while (i < len)
// 	{
// 		word = ft_charjoin(word, s[i]);
// 		i++;
// 	}
// 	return (word);
// }

// static void	ft_clean(char **str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i] != NULL)
// 	{
// 		free(str[i]);
// 		i++;
// 	}
// 	free(str);
// }

// char	**ft_split_argv(char *s, char c)
// {
// 	char	**res;
// 	int		word_count;
// 	int		offset;
// 	size_t	i;

// 	res = NULL;
// 	word_count = ft_count_words(s, c);
// 	if (word_count == -1) // if the quotes aren't closed
// 	{
// 		printf("%s: open quotes are not supported\n", PROG_NAME); //ft_error_handler();
// 		return (NULL);
// 	}
// 	res = (char **)malloc(sizeof(char **) * (word_count + 1));
// 	if (res == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (*s != '\0')
// 	{
// 		if (*s != c)
// 		{
// 			offset = ft_offset_str(s, c);
// 			res[i] = ft_copy_word(s, offset);
// 			if (res[i] == NULL)
// 			{
// 				ft_clean(res); //ft_error_handler(); 1 //malloc failed
// 				return (NULL);
// 			}
// 			s += offset;
// 			i++;
// 		}
// 		else
// 			s++;
// 	}
// 	res[i] = NULL;
// 	return (res);
// }

char	**ft_split_argv(char **args)
{
	int		i;
	int		z;
	int		y;
	int		size;
	char	**new_args;
	char	**temp;

	i = 0;
	y = 0;
	size = 0;
	while(args[i])
	{
		size += ft_count_words(args[i], ' ');
		i++;
	}
	new_args = (char **) malloc((size + 1) * sizeof(char));
	if (!new_args)
	{
		ft_error_handler();
	}
	i = 0;
	while(args[i])
	{
		temp = ft_split(args[i], ' ');
		z = 0;
		while (temp[z])
		{
			new_args[y] = temp[z];
			z++;
			y++;
		}
		free(temp);
		i++;
	}
	new_args[y] = NULL;
	ft_free_vector(args);
	return (new_args);
}
