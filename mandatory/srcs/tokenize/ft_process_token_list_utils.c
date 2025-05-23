/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_token_list_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:49:18 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 18:49:19 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Expands an environment variable found in a string.
 * 
 * This function extracts an environment variable name from the input string
 * and searches for its value in the environment variable array. If found, it
 * returns the corresponding value; otherwise, it returns an empty string. It
 * updates the index to skip past the environment variable name.
 * 
 * @param s Pointer to the current position in the input string.
 * @param i Pointer to the index, which is updated to skip past the variable
 *        name.
 * @param envp Array of environment variables.
 * @return The expanded environment variable value or an empty string.
 */
static char	*ft_expand_env(char *s, int *i, char **envp)
{
	char	*expansion;
	char	*env;
	char	*env_equal;
	int		len;
	int		j;

	len = 0;
	while (ft_isalpha(s[len]) || ft_isdigit(s[len]) || s[len] == '_')
		len++;
	env = ft_substr(s, 0, len);
	env_equal = ft_strjoin(env, "=");
	j = 0;
	while (envp[j])
	{
		if (ft_strncmp(env_equal, envp[j], (len + 1)) == 0)
		{
			expansion = ft_strdup(envp[j] + (len + 1));
			(*i) += len;
			return (free(env_equal), free(env), expansion);
		}
		j++;
	}
	(*i) += len;
	return (free(env_equal), free(env), ft_strdup("\0"));
}

/**
 * @brief Retrieves the exit code as a string.
 * 
 * This function retrieves the last process's exit code. It updates the index
 * and returns the exit_code in string format.
 * 
 * @param i Pointer to the index, incremented to skip past the special
 *        character.
 * @return Exit code value in string format.
 */
static char	*ft_get_exit_code(int *i)
{
	int		status_code;
	char	*status_str;

	(*i)++;
	status_code = ft_exit_status(0, FALSE, FALSE);
	status_str = ft_itoa(status_code);
	return (status_str);
}

/**
 * @brief Handles variable in a string during token parsing.
 * 
 * This function processes environment variables and exit codes within a given
 * token. It identifies the type of expansion based on the character following a
 * `$` symbol and appends the expanded value to the `new_value` string. For `?`,
 * it expands the last exit code, else, it expands the enviroment variable.
 * 
 * @param new_value A pointer to the current expanded string being constructed. 
 *        This will be updated with the appended expanded value.
 * @param value The original string containing the variable to expand.
 * @param i A pointer to the current position index within `value`. This index
 *        will be updated to point past the processed expansion.
 * @param envp The environment variables array used for expanding variable
 *        names.
 */
void	ft_handle_expansion(char **new_value, char *value, int *i, char **envp)
{
	char	*expansion;
	char	*tmp;

	tmp = NULL;
	(*i)++;
	if (value[*i] == '?')
		expansion = ft_get_exit_code(i);
	else
		expansion = ft_expand_env(&value[*i], i, envp);
	tmp = ft_strjoin(*new_value, expansion);
	free(*new_value);
	free(expansion);
	*new_value = tmp;
}

/**
 * @brief Processes single-quoted text in a string.
 * 
 * This function appends characters within single quotes to `new_value`,
 * preserving the quotes,and updates the index to skip past the quoted section.
 * 
 * @param new_value Pointer to the string being built.
 * @param value Original input string.
 * @param i Pointer to the index, updated to skip past the quoted section.
 */
void	ft_handle_squotes(char **new_value, char *value, int *i)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[*i] && value[*i] != SQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
}

/**
 * @brief Processes double-quoted text with potential variable expansion.
 * 
 * This function appends characters within double quotes to `new_value`,
 * performing environment variable expansion for `$` symbols where applicable.
 * It updates the index to skip past the quoted section.
 * 
 * @param new_value Pointer to the string being built with expansions.
 * @param value Original input string.
 * @param i Pointer to the index, updated to skip past the quoted section.
 * @param envp Array of environment variables.
 */
void	ft_handle_dquotes(char **new_value, char *value, int *i, char **envp)
{
	*new_value = ft_charjoin(*new_value, value[(*i)++]);
	while (value[*i] && value[*i] != DQUOTE)
	{
		if (value[*i] == '$' && ft_is_expandable(&value[*i + 1]))
			ft_handle_expansion(new_value, value, i, envp);
		else
			*new_value = ft_charjoin(*new_value, value[(*i)++]);
	}
	if (value[*i] == DQUOTE)
		*new_value = ft_charjoin(*new_value, value[(*i)++]);
}
