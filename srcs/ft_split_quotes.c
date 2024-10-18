#include "../includes/minishell.h"

static int	ft_next_quote(char *s, int i, char c)
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
			if (s[i] == 39)
			{
				i = ft_next_quote(s, i, 39);
				if (i == -1)
					return (-1);
			}
			// if (s[i] == 34) // nao sei como testar isso ainda
			// {
			// 	i = ft_next_quote(s, i, 34);
			// 	if (i == -1)
			// 		return (-1);
			// }
		}
		if (s[i] == c)
			in_word = false;
		i++;
	}
	return (count);
}

char	**ft_split_quotes(char *s, char c)
{
	char	**res;
	int		word_count;

	res = NULL;
	word_count = ft_count_words(s, c);
	res = (char **)malloc(sizeof(char **) * (word_count + 1));
	if (res == NULL)
		return (NULL);

	printf("%d\n", word_count);
	return (res);
}