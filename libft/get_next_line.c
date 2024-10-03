/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:11:35 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/06 16:40:23 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strmerge(char *stash, char *buf)
{
	char	*new_stash;

	if (*stash == '\0' && *buf == '\0')
	{
		free(stash);
		free(buf);
		return (NULL);
	}
	new_stash = ft_strjoin(stash, buf);
	if (stash)
		free(stash);
	if (new_stash == NULL)
		return (NULL);
	return (new_stash);
}

static char	*ft_read_into_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bc;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf == NULL)
		return (NULL);
	*buf = '\0';
	bc = BUFFER_SIZE;
	while (bc != 0 && ft_strchr(buf, '\n') == NULL)
	{
		bc = read(fd, buf, BUFFER_SIZE);
		if (bc < 0)
		{
			free(stash);
			free(buf);
			return (NULL);
		}
		buf[bc] = '\0';
		stash = ft_strmerge(stash, buf);
		if (stash == NULL)
			return (NULL);
	}
	free(buf);
	return (stash);
}

char	*ft_get_line(char *stash)
{
	char	*line;
	size_t	line_len;
	size_t	i;

	if (ft_strchr(stash, '\n') == NULL)
		line_len = ft_strlen(stash);
	else
		line_len = ft_strlen(stash) - ft_strlen(ft_strchr(stash, '\n') + 1);
	line = (char *)malloc(sizeof(char) * line_len + 1);
	if (line == NULL)
		return (NULL);
	i = 0;
	while (i < line_len)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*ft_update_stash(char *stash)
{
	char	*new_stash;

	if (ft_strchr(stash, '\n') == NULL)
	{
		free(stash);
		return (NULL);
	}
	new_stash = ft_strdup(ft_strchr(stash, '\n') + 1);
	free(stash);
	if (new_stash[0] == '\0')
		return (free(new_stash), NULL);
	if (new_stash == NULL)
		return (NULL);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash[OPENFD_MAX];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (stash[fd] == NULL)
	{
		stash[fd] = (char *)malloc(sizeof(char));
		if (stash[fd] == NULL)
			return (NULL);
		*stash[fd] = '\0';
	}
	stash[fd] = ft_read_into_stash(fd, stash[fd]);
	if (stash[fd] == NULL)
		return (NULL);
	line = ft_get_line(stash[fd]);
	stash[fd] = ft_update_stash(stash[fd]);
	return (line);
}
