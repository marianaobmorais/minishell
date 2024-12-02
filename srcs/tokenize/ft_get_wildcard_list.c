#include "../../includes/minishell.h"

char	*ft_get_prefix(char *s)
{
	char	*prefix;
	int		i;

	i = 0;
	prefix = NULL;
	while (s[i] != '*')
	{
		prefix = ft_charjoin(prefix, s[i]);
		i++;
	}
	return (prefix);
}

char *ft_get_sufix(char *s)
{
	char	*sufix;
	char	*tmp;
	int		i;

	i = 0;
	sufix = NULL;
	tmp = ft_strrchr(s, '*');
	while (tmp[i + 1])
	{
		sufix = ft_charjoin(sufix, tmp[i + 1]);
		i++;
	}
	return (sufix);
}

int	ft_strncmp_(const char *str1, const char *str2, size_t len)
{
	unsigned char	*s1;
	unsigned char	*s2;

	if (!str1 || !str2) //added this check
		return (0);
	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while ((*s1 != '\0' || *s2 != '\0') && (len > 0))
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		++s1;
		++s2;
		--len;
	}
	return (0);
}

void	ft_update_token_list(t_list *curr, t_list *prev, t_list **head, t_list **w_list)
{
	t_list	*next; //write brief
	t_list	*last_node;

	next = curr->next;
	ft_free_content(curr);
	if (prev)
		prev->next = *w_list;
	else
		*head = *w_list; //need to replace the list node
	if (*w_list)
	{
		last_node = ft_lstlast(*w_list);
		last_node->next = next;
	}
}

bool	ft_validate_entry(char *s, char *entry_name)
{
	char	*prefix; //write brief
	char	*sufix;
	char	*substring;
	bool	result;

	result = false;
	prefix = ft_get_prefix(s);
	sufix = ft_get_sufix(s);
	substring = ft_substr(entry_name, ft_strlen(entry_name) - ft_strlen(sufix),
		ft_strlen(sufix));
	if (ft_strncmp(entry_name, prefix, ft_strlen(prefix)) == 0
		&& ft_strncmp_(substring, sufix, ft_strlen(sufix)) == 0)
		result = true;
	free(substring);
	free(prefix);
	free(sufix);
	return (result);
}

t_list	**ft_get_wildcard_list(char *s)
{
	t_list			**wild_list;
	char			dir_path[1024];
	char			*entry_name;
	DIR				*dir;
	struct dirent	*entry;

	if (!getcwd(dir_path, 1024))
	{
		perror("getcwd"); //error_handler;
		return (NULL);
	}
	dir = opendir(dir_path);
	if (!dir)
	{
		perror("opendir"); //error_handler;
		return (NULL);
	}
	wild_list = (t_list **)malloc(sizeof(t_list *));
	if (!wild_list)
		return (NULL); //error_handler; 1 //malloc failed
	*wild_list = NULL;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.') //ignore hidden files and directories
		{
			if (ft_validate_entry(s, entry->d_name))
			{
				entry_name = ft_strdup(entry->d_name);
				if (!entry_name)
				{
					perror("ft_strdup");
					continue; // Handle strdup failure gracefully
				}
				ft_add_to_token_list(&entry_name, wild_list);
				free(entry_name);
			}
		}
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		perror("closedir"); //error_handler;
	return (wild_list);
}
