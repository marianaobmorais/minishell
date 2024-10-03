/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:44:19 by mariaoli          #+#    #+#             */
/*   Updated: 2024/04/20 13:33:31 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	count;
	int		inword;

	count = 0;
	inword = 0;
	while (*s != '\0')
	{
		if (*s == c)
			inword = 0;
		else if (inword == 0)
		{
			inword = 1;
			++count;
		}
		++s;
	}
	return (count);
}

static char	*ft_copyword(char const *s, char c)
{
	size_t	i;
	size_t	len;
	char	*word;

	len = 0;
	while (s[len] != '\0' && s[len] != c)
		++len;
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (word == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		++i;
	}
	word[i] = '\0';
	return (word);
}

static void	ft_clean_all(char **strings)
{
	int	i;

	i = 0;
	while (strings[i] != NULL)
	{
		free(strings[i]);
		i++;
	}
	free(strings);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	char	**res;

	res = (char **)malloc(sizeof(char *) * (ft_wordcount(s, c) + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			res[i] = ft_copyword(s, c);
			if (res[i++] == NULL)
			{
				ft_clean_all(res);
				return (NULL);
			}
			while (*s != '\0' && *s != c)
				++s;
		}
		else
			++s;
	}
	res[i] = NULL;
	return (res);
}
