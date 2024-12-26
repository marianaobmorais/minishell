/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_wildcard_list_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:45:31 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/26 15:33:53 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Determines if the given string contains a prefix before the first '*'.
 *
 * Checks whether the string starts with a character other than '*' to identify
 * if a prefix exists. If the first character is not '*', a prefix is assumed.
 *
 * @param s The string to be checked.
 * @return `true` if a prefix exists, otherwise `false`.
 */
static bool	ft_find_prefix(char *s)
{
	int		i;

	i = 0;
	if (!s)
		return (false);
	if (s[i] != '*')
		return (true);
	else
		return (false);
}

/**
 * @brief Validates an entry name against a wildcard pattern.
 *
 * Checks if the entry name matches the conditions defined by the wildcard 
 * pattern `s`. The function validates prefix, middle, and suffix matches in
 * sequence.
 *
 * @param s The wildcard pattern to validate against.
 * @param entry_name The name of the directory entry to validate.
 * @return `true` if the entry name matches the pattern, otherwise `false`.
 */
static bool	ft_validate_entry(char *s, char *entry_name)
{
	bool	result;

	result = true;
	if (!s || !entry_name)
		return (false);
	if (ft_find_prefix(s))
	{
		result = ft_match_prefix(&s, &entry_name);
		if (!result)
			return (result);
	}
	if (ft_find_middle(s))
	{
		result = ft_match_middle(&s, &entry_name);
		if (!result)
			return (result);
	}
	if (ft_find_sufix(s))
	{
		result = ft_match_sufix(&s, &entry_name);
		if (!result)
			return (result);
	}
	return (result);
}

/**
 * @brief Processes a directory entry and adds it to the wildcard list if valid.
 *
 * Validates the directory entry name against the wildcard pattern `s` and 
 * adds it to the wildcard list if it matches. Skips hidden files and saves 
 * wildcard-matching entries in a token list.
 *
 * @param s The wildcard pattern used for validation.
 * @param entry A pointer to the directory entry to process.
 * @param wild_list A pointer to the list where valid entries are added.
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
 * @brief Generates a list of entries matching a wildcard pattern.
 *
 * Opens the current directory, processes its entries against the wildcard 
 * pattern `s`, and creates a list of valid entries. Ensures that the final 
 * list is sorted alphabetically.
 *
 * @param s The wildcard pattern to match against directory entries.
 * @return A pointer to the list of matching entries, or `NULL` on failure.
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
	wild_list = ft_set_alphabetical_order(wild_list);
	return (wild_list);
}
