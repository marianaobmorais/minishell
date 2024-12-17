/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_wildcard_list_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:24:38 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 19:24:39 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Validates if a directory entry matches a wildcard pattern.
 * 
 * This function checks if the given `entry_name` starts with the prefix,
 * has a predetermined middle and ends with the suffix extracted from the
 * wildcard pattern `s`. It uses helper functions to extract the prefix and
 * suffix and performs comparisons to validate the match.
 * 
 * @param s The wildcard pattern containing `*` for matching.
 * @param entry_name The directory entry name to validate.
 * @return `true` if the `entry_name` matches the wildcard pattern, otherwise
 *         `false`.
 */
static bool	ft_validate_entry(char *s, char *entry_name)
{
	char	*prefix;
	char	*sufix;
	char	*middle;
	char	*substring;
	bool	result;

	result = false;
	prefix = ft_get_prefix(s);
	sufix = ft_get_sufix(s);
	middle = ft_get_middle(s);
	substring = ft_substr(entry_name, ft_strlen(entry_name) - ft_strlen(sufix),
			ft_strlen(sufix));
	if (ft_strncmp(entry_name, prefix, ft_strlen(prefix)) == 0
		&& ft_strncmp_(substring, sufix, ft_strlen(sufix)) == 0
		&& ft_strnstr_(entry_name, middle, ft_strlen(entry_name)) != NULL)
		result = true;
	if (substring)
		free(substring);
	if (prefix)
		free(prefix);
	if (sufix)
		free(sufix);
	if (middle)
		free(middle);
	return (result);
}

/**
 * @brief Replaces a token in the token list with a list of wildcard-expanded
 *        tokens.
 * 
 * This function replaces a given node (`curr`) in a linked list with a list of 
 * expanded wildcard tokens (`wild_list`). It correctly adjusts the `prev`
 * pointer and updates the `head` pointer if the replaced node is the head of
 * the list. Any remaining nodes after `curr` are linked to the end of the new
 * `wild_list`.
 * 
 * @param curr The current node in the token list being replaced. Its content 
 *        will be freed, and the node will be removed.
 * @param prev The previous node in the list before `curr`. This is updated to 
 *        point to the first node of `wild_list`. If `curr` is the head, this 
 *        parameter should be `NULL`.
 * @param head A pointer to the head of the list. This will be updated if `curr`
 *        is the first node.
 * @param wild_list The list of expanded wildcard tokens to replace `curr`.
 *        This list is linked into the main token list.
 */
void	ft_update_token_list(t_list *curr, t_list *prev, t_list **head, \
	t_list **wild_list)
{
	t_list	*next;
	t_list	*last_node;

	next = curr->next;
	ft_free_content(curr);
	if (prev)
		prev->next = *wild_list;
	else
		*head = *wild_list;
	if (*wild_list)
	{
		last_node = ft_lstlast(*wild_list);
		last_node->next = next;
	}
}

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
 * @brief Processes a directory entry and adds it to the wildcard list if valid
 *
 * This function evaluates a directory entry against a wildcard pattern and, if
 * valid, creates a new node and appends it to the wildcard list. Skips hidden 
 * files and directories (entries starting with a dot `.`). It also marks the
 * `wildcard` attribute of the node as `false` if the node contains a wildcard,
 * preventing recursive wildcard expansion.
 *
 * @param s The wildcard pattern to validate against.
 * @param entry The directory entry (`struct dirent`) to process.
 * @param wild_list A pointer to the wildcard list to which new nodes are added
 */
static void	ft_process_entry(char *s, struct dirent *entry, t_list **wild_list)
{
	char	*entry_name;
	t_list	*new_node;

	if (entry->d_name[0] == '.')
		return ;
	if (!ft_validate_entry(s, entry->d_name))
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
 * @brief Retrieves a list of directory entries matching a wildcard pattern.
 * 
 * This function opens the current working directory and iterates through its 
 * contents, matching each entry against the wildcard pattern `s`. Matching 
 * entries are added to a linked list. The directory is closed before returning
 * 
 * @param s The wildcard pattern to match directory entries against.
 * @return A pointer to a linked list (`t_list **`) of matching directory
 *         entries. Returns `NULL` on failure.
 */
t_list	**ft_get_wildcard_list(char *s)
{
	t_list			**wild_list;
	char			dir_path[1024];
	DIR				*dir;
	struct dirent	*entry;

	dir = ft_open_directory(dir_path);
	if (!dir)
		return (ft_stderror(TRUE, ""), ft_exit_status(1, TRUE, FALSE), NULL);
	wild_list = (t_list **)malloc(sizeof(t_list *));
	if (!wild_list)
		return (closedir(dir), ft_error_malloc("wild_list"), NULL);
	*wild_list = NULL;
	entry = readdir(dir);
	while (entry)
	{
		ft_process_entry(s, entry, wild_list);
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
	{
		ft_stderror(TRUE, "closedir");
		ft_exit_status(1, TRUE, FALSE);
	}
	return (wild_list);
}
