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
		if (s[i] == 34)
		{
			i = ft_next_quote(s, i, 34);
			if (i == -1)
				return (-1);
		}
		if (s[i] == c)
			in_word = false;
		i++;
	}
	return (count);
}

int	ft_offset_str(char *s, char c)
{
	int		len;

	len = 0;
	while (s[len] != '\0' && s[len] != c)
	{
		if (s[len] == 39) 
			len = ft_next_quote(s, len, 39);
		else if (s[len] == 34)
			len = ft_next_quote(s, len, 34);
		if (s[len] != c)
			len++;
	}
	return (len);
}

char	*ft_copy_word(char *s, int len)
{
	char	*word;
	int		i;
	int		j;

	word = (char *)malloc(sizeof(char *) * (len + 1));
	if (word == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (j < len)
	{
		if (s[j] == 39)
		{
			j++;
			while (s[j] != 39)
			{
				word[i] = s[j];
				i++;
				j++;
			}
			j++;
		}
		else if (s[j] == 34)
		{
			j++;
			while (s[j] != 34)
			{
				word[i] = s[j];
				i++;
				j++;
			}
			j++;
		}
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

static void	ft_clean(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	**ft_split_quotes(char *s, char c)
{
	char	**res;
	int		word_count;
	int		offset;
	size_t	i;

	res = NULL;
	word_count = ft_count_words(s, c);
	if (word_count == -1) // if the quotes aren't closed
	{
		printf("%s open quotes are not accepted\n", PROMPT); //ft_error_handler();
		return (NULL);
	}
	printf("word count = %d\n", word_count); // delete later
	res = (char **)malloc(sizeof(char **) * (word_count + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			offset = ft_offset_str(s, c);
			res[i] = ft_copy_word(s, offset);
			if (res[i] == NULL)
			{
				ft_clean(res);
				return (NULL);
			}
			printf("res[%zu] = %s\n", i, res[i]); // delete later
			s += offset;
			i++;
		}
		else
			s++;
	}
	res[i] = NULL;
	return (res);
}
