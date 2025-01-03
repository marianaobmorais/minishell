/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_token_list_utils2_bonus.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:25:44 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 19:25:45 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Determines if a string represents an expandable environment or status
 *        variable.
 * 
 * Checks if the string `s` begins with a valid environment variable name
 * (alphabetic character or underscore followed by an alphanumeric character)
 * or with `$?`, indicating the exit status. If so, the function returns `true`
 * indicating that the string can be expanded; otherwise, it returns `false`.
 * 
 * @param s Pointer to the string to check for expandability.
 * @return true if `s` represents an expandable variable, otherwise false.
 */
bool	ft_is_expandable(char *s)
{
	int	i;

	i = 0;
	if (ft_isalpha(s[i]) || s[i] == '?' || s[i] == '*'
		|| (s[i] == '_' && s[i + 1] && ft_isalnum(s[i + 1])))
		return (true);
	return (false);
}

/**
 * @brief Removes leading and trailing quotes from a token's value.
 * 
 * This function iterates through a token's value string, 
 * removing both leading and trailing single or double quotes encountered.
 * 
 * @param tkn Pointer to the token structure containing the value to process.
 */
void	ft_remove_quotes(t_token *tkn)
{
	char	quote;
	int		i;

	i = 0;
	while (tkn->value[i])
	{
		if (tkn->value[i] == '$'
			&& (tkn->value[i + 1] == SQUOTE || tkn->value[i + 1] == DQUOTE))
			ft_memmove(&tkn->value[i], &tkn->value[i + 1],
				ft_strlen(&tkn->value[i + 1]) + 1);
		if (tkn->value[i] == SQUOTE || tkn->value[i] == DQUOTE)
		{
			quote = tkn->value[i];
			ft_memmove(&tkn->value[i], &tkn->value[i + 1],
				ft_strlen(&tkn->value[i + 1]) + 1);
			while (tkn->value[i] && tkn->value[i] != quote)
				i++;
			if (tkn->value[i] == quote)
				ft_memmove(&tkn->value[i], &tkn->value[i + 1],
					ft_strlen(&tkn->value[i + 1]) + 1);
		}
		else
			i++;
	}
	return ;
}

/**
 * @brief Expands environment variables in a token's value.
 * 
 * This function iterates through each character in a token's value. If it
 * encounters single quotes, expansion doesn't occur. If it finds a `$`
 * followed by an expandable identifier, that is either between double quotes
 * or none, it replaces it with the corresponding environment variable value
 * The resulting expanded string is assigned to the token's value.
 * 
 * @param token Pointer to the token to be expanded.
 * @param envp Environment variable array used for expansion.
 */
void	ft_expand_tokens(t_token *token, char **envp)
{
	char	*new_value;
	int		i;

	new_value = NULL;
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == SQUOTE)
			ft_handle_squotes(&new_value, token->value, &i);
		else if (token->value[i] == DQUOTE)
			ft_handle_dquotes(&new_value, token->value, &i, envp);
		else if (token->value[i] == '$'
			&& ft_is_expandable(&token->value[i + 1]))
			ft_handle_expansion(&new_value, token->value, &i, envp);
		else if (token->value[i] && token->value[i] != DQUOTE
			&& token->value[i] != SQUOTE)
			new_value = ft_charjoin(new_value, token->value[i++]);
	}
	free(token->value);
	token->value = new_value;
}
