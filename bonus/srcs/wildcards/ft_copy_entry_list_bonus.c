/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_copy_entry_list_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:45:28 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/23 16:45:29 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Opens the current working directory for reading its contents.
 * 
 * This function retrieves the current working directory using `getcwd` and 
 * opens it using `opendir`. If `getcwd` fails, it logs the error and returns
 * `NULL`.
 * 
 * @param dir_path A buffer to store the path of the current working directory. 
 *        It must have a size of at least 1024 bytes.
 * @return A pointer to the `DIR` structure representing the opened directory 
 *         on success, or `NULL` on failure.
 */
static DIR	*ft_open_directory(char *dir_path)
{
	if (!getcwd(dir_path, 1024))
	{
		ft_stderror(TRUE, "getcwd");
		ft_exit_status(1, TRUE, FALSE);
		return (NULL);
	}
	return (opendir(dir_path));
}

/**
 * @brief Adds a directory entry to the wildcard list.
 *
 * This function checks if a directory entry is not hidden (i.e., its name 
 * does not start with a dot) and creates a token from the entry name. 
 * It then adds the token to the wildcard list. If the entry name matches a 
 * wildcard pattern, the wildcard flag in the token is set to false.
 *
 * @param entry A pointer to the directory entry to be processed.
 * @param wild_list A pointer to the wildcard list where the entry will be added
 */
static void	ft_add_to_entry_list(struct dirent *entry, t_list **wild_list)
{
	char	*entry_name;
	t_list	*new_node;

	if (entry->d_name[0] == '.')
		return ;
	entry_name = ft_strdup(entry->d_name);
	if (!entry_name)
		return (ft_error_malloc("entry_name"));
	ft_add_to_token_list(&entry_name, wild_list);
	new_node = ft_lstlast(*wild_list);
	if (ft_is_wildcard(((t_token *)new_node->content)->value))
		((t_token *)new_node->content)->wildcard = false;
}

/**
 * @brief Copies all non-hidden directory entries into a sorted token list.
 *
 * This function opens the current directory, iterates through its entries, 
 * and adds non-hidden entry names to a token list. In case of errors during 
 * directory operations or memory allocation, appropriate error handling is 
 * performed.
 *
 * @return A pointer to a list of tokens representing the directory entries, 
 * or `NULL` if an error occurred.
 */
t_list	**ft_copy_entry_list(void)
{
	t_list			**entry_list;
	char			dir_path[1024];
	DIR				*dir;
	struct dirent	*entry;

	dir = ft_open_directory(dir_path);
	if (!dir)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), NULL);
	entry_list = (t_list **)malloc(sizeof(t_list *));
	if (!entry_list)
		return (closedir(dir), ft_error_malloc("entry_list"), NULL);
	*entry_list = NULL;
	entry = readdir(dir);
	while (entry)
	{
		ft_add_to_entry_list(entry, entry_list);
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
	{
		ft_stderror(TRUE, "closedir");
		ft_exit_status(1, TRUE, FALSE);
	}
	return (entry_list);
}
