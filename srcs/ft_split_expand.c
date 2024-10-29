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
			i = ft_next_quote(s, i, 39); // single quote
			if (i == -1)
				return (-1);
		}
		if (s[i] == 34)
		{
			i = ft_next_quote(s, i, 34); // double quote
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

char	*ft_expand_env(char *s, char **my_envp)
{
	char	*word;
	char	*env;
	char	*env_equal;
	int		len;
	int		i;

	i = 0;
	if (s[i] == '?') // to do
	{
		printf("exit code goes here\n");
	}
	while (ft_isalpha(s[i]))
		i++;
	env = ft_substr(s, 0, i);
	env_equal = ft_strjoin(env, "=");
	len = ft_strlen(env_equal);
	//printf("env = %s, len = %d\n", env_equal, len);//
	i = 0;
	while (my_envp[i])
	{
		if (ft_strncmp(env_equal, my_envp[i], len) == 0)
		{
			word = ft_strdup(my_envp[i] + len);
			return (free(env_equal), word);
		}
		i++;
	}
	return (free(env_equal), ft_strdup("\0"));
}

char	*ft_copy_word(char *s, int len, char **my_envp)
{
	char	*word;
	char	*expansion;
	char	*tmp;
	int		i;

	word = NULL;
	i = 0;
	while (i < len)
	{
		if (s[i] == 39) // single quote prevents shell from interpreting metacharacters
		{
			i++;
			while (s[i] != 39)
			{
				word = ft_charjoin(word, s[i]);
				i++;
			}
			i++;
		}
		else if (s[i] == 34) // double quotes prevents shell from interpreting metacharacters except $
		{
			i++;
			while (s[i] != 34)
			{
				if (s[i] == '$')
				{
					i++;
					expansion = ft_expand_env(&s[i], my_envp);
					tmp = ft_strjoin(word, expansion);
					free(word);
					word = tmp;
					while (ft_isalpha(s[i]))
						i++;
				}
				else
				{
					word = ft_charjoin(word, s[i]);
					i++;
				}
			}
			i++;
		}
		else if (s[i] == '$')
		{
			i++;
			word = ft_expand_env(&s[i], my_envp);
			//printf("%s\n", word);
			if (s[i] == '?')
				i++;
			else
			{
				while (ft_isalpha(s[i]))
					i++;
			}
		}
		else
		{
			word = ft_charjoin(word, s[i]);
			i++;
		}
	}
	word = ft_charjoin(word, '\0');
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

char	**ft_split_expand(char *s, char c, char **my_envp)
{
	(void)my_envp;
	char	**res;
	int		word_count;
	int		offset;
	size_t	i;

	res = NULL;
	word_count = ft_count_words(s, c);
	if (word_count == -1) // if the quotes aren't closed
	{
		printf("%s open quotes are not supported\n", PROMPT); //ft_error_handler();
		return (NULL);
	}
	res = (char **)malloc(sizeof(char **) * (word_count + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			offset = ft_offset_str(s, c);
			res[i] = ft_copy_word(s, offset, my_envp);
			if (res[i] == NULL)
			{
				ft_clean(res);
				return (NULL);
			}
			s += offset;
			i++;
		}
		else
			s++;
	}
	res[i] = NULL;
	return (res);
}
