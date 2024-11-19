#include "../../includes/minishell.h"

/**
 * @brief Adds a new string to a dynamically allocated string vector.
 * 
 * Creates a new vector by appending a given string to an existing vector of strings.
 * Frees the original vector and duplicates its contents into the new one.
 * 
 * @param vector Pointer to the original string vector, or NULL if creating a new vector.
 * @param new_str String to add to the vector.
 * @return A pointer to the new vector with the appended string, or NULL if allocation fails.
 */
/* t_list	**ft_add_to_vector(char **vector, char *new_str) //not used
{
	char	**res;
	int		i;

	i = 0;
	if (vector)
		while (vector[i])
			i++;
	res = (char **)malloc(sizeof(char *) * (i + 2));
	if (!res)
		return (NULL); //ft_error_handler // malloc failed
	i = 0;
	if (!vector)
		res[i++] = ft_strdup(new_str); //malloc check? free allocated mem?
	else
	{
		while (vector[i])
		{
			res[i] = ft_strdup(vector[i]); //malloc check? free allocated mem?
			i++;
		}
		res[i++] = ft_strdup(new_str); //malloc check? free allocated mem?
		ft_free_vector(vector);
	}
	res[i] = NULL;
	return (res);
} */

/**
 * @brief Extracts a list of argument strings from a token list.
 * 
 * Iterates through a token list, collecting values of tokens with types `EXEC`, `EXPORT`, 
 * or `EXPORT_AP` into a dynamically allocated string vector. Skips over redirection tokens 
 * and stops at a pipe (`PIPE`) token or the end of the list.
 * 
 * @param list A double pointer to the token list.
 * @return A dynamically allocated string vector containing the extracted arguments, 
 *         or NULL if no arguments are found.
 */
t_list	**ft_get_args(t_list **list)
{
	t_list	**args;
	t_list	*curr;
	t_token	*token;

	args = (t_list **)malloc(sizeof(t_list *)); //create t_list **
	if (!args)
		return (NULL); // ft_error_handler();
	*args = NULL;
	curr = *list;
	while (curr)
	{
		token = (t_token *)curr->content;
		if (token->type == EXEC || token->type == EXPORT || token->type == EXPORT_AP)
			ft_lstadd_back(args, curr);
		else if (token->type == APPEND || token->type == OUTFILE || token->type == HEREDOC || token->type == INFILE)
		{
			if (curr->next && ((t_token *)curr->next->content)->type != PIPE)
				curr = curr->next;
		}
		else if (token->type == PIPE)
			break ;
		curr = curr->next;
	}
	return (args);
}
/**
 * @brief Searches for the next redirection token in the token list.
 * 
 * Scans the token list to find the next redirection token (`OUTFILE`, `INFILE`, `APPEND`, 
 * or `HEREDOC`). Stops scanning at a pipe (`PIPE`) token or the end of the list.
 * 
 * @param list A double pointer to the token list, updated as tokens are consumed during the search.
 * @return true if a redirection token is found, otherwise false.
 */
bool	ft_find_next_redir(t_list **list)
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
			break ;
		if (token->type == OUTFILE || token->type == INFILE
				|| token->type == APPEND || token->type == HEREDOC)
			return (true);
		*list = (*list)->next;
	}
	return (false);
}

/**
 * @brief Searches for the next executable token in the token list.
 * 
 * Scans the token list to find the next executable token (`EXEC`). Stops scanning 
 * at a pipe (`PIPE`) token or the end of the list.
 * 
 * @param list A double pointer to the token list, updated as tokens are consumed during the search.
 * @return true if an executable token is found, otherwise false.
 */
bool	ft_find_next_exec(t_list **list)
{
	t_token	*token;

	while (*list)
	{
		token = (t_token *)(*list)->content;
		if (token->type == PIPE)
			break ;
		if (token->type == EXEC)
			return (true);
		*list = (*list)->next;
	}
	return (false);
}
