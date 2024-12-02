#include "../../includes/minishell.h"

void	ft_expand_wildcard(t_list *current, t_list	*prev, t_list **head)
{
	t_list			**wild_list;
	t_list			*wild_last;
	t_list			*next;
	char			dir_path[1024];
	char			*entry_name;
	DIR				*dir;
	struct dirent	*entry;

	if (!getcwd(dir_path, 1024))
	{
		perror("getcwd"); //error_handler;
		return ;
	}
	dir = opendir(dir_path);
	if (!dir)
	{
		perror("opendir"); //error_handler;
		return ;
	}
	wild_list = (t_list **)malloc(sizeof(t_list *));
	if (!wild_list)
		return ;  //error_handler; 1 //malloc failed
	*wild_list = NULL;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.') //ignore hidden files and directories
		{
			entry_name = ft_strdup(entry->d_name);
			if (!entry_name)
			{
				perror("ft_strdup");
				continue; // Handle strdup failure gracefully
			}
			ft_add_to_token_list(&entry_name, wild_list);
		}
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		perror("closedir"); //error_handler;
	next = current->next;
	ft_free_content(current);
	if (prev)
		prev->next = *wild_list;
	else
		*head = *wild_list; //need to replace the list node
	if (*wild_list)
	{
		wild_last = ft_lstlast(*wild_list);
		wild_last->next = next;
	}
	free(wild_list);
}