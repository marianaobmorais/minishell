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
		}
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
		if (s[i] == c)
			in_word = false;
		i++;
	}
	return (count);
}

char	*ft_copy_word(char *s, char c)
{
	char	*word;
	int		len;
	int		i;
	int		j;

	len = 0;
	while (s[len] != '\0' && s[len] != c)
	{
		if (s[len] == 39) // and s[len] == 34?
			len = ft_next_quote(s, len, 39);
		if (s[len] != c)
			len++;
	}
	word = (char *)malloc(sizeof(char *) * (len + 1));
	if (word == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (j < len)
	{
		if (s[j] == 39)
			j++;
		else
		{
			word[i] = s[j];
			i++;
			j++;
		}
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split_quotes(char *s, char c)
{
	char	**res;
	int		word_count;
	size_t	i;

	res = NULL;
	word_count = ft_count_words(s, c);
	if (word_count == -1) // if the quotes aren't closed
		return (NULL);
	printf("word count = %d\n", word_count);
	res = (char **)malloc(sizeof(char **) * (word_count + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			res[i] = ft_copy_word(s, c);
			// if (res[i] == NULL)
			// {
			// 	ft_clean();
			// 	return (NULL);
			// }
			printf("res[%zu] = %s\n", i, res[i]);
			s += ft_strlen(res[i]) + 1;
			if (*s == 39) // || *s == 34
				s++;
			i++;
		}
		else
			s++;
	}
	res[i] = NULL;
	return (res);
}