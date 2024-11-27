#include "../../includes/minishell.h"

/**
 * @brief Checks if a token matches a specified type.
 * 
 * This function checks whether the given token matches one of several
 * predefined types, depending on the value of the `type` parameter. The
 * function will return `true` if the token matches any of the types in the
 * specified category (e.g., redirection types, execution types, or logical
 * node types). If the token doesn't match the specified category, it returns
 * `false`.
 * 
 * The function supports checking the following categories:
 * - `REDIR`: checks for output redirection (`OUTFILE`, `INFILE`, `APPEND`,
 *            `HEREDOC`);
 * - `EXEC`: checks for execution-related types (`EXEC`, `EXPORT`,`EXPORT_AP`);
 * - `NODE`: checks for logical operators (`PIPE`, `AND`, `OR`)
 * 
 * @param token The token to check.
 * @param type The type category to check the token against.
 * @return `true` if the token matches the specified type, `false` otherwise.
 */
bool	ft_is_token_type(t_token *token, t_type type)
{
	if (type == REDIR)
	{
		if (token->type == OUTFILE || token->type == INFILE
				|| token->type == APPEND || token->type == HEREDOC)
			return (true);
	}
	if (type == EXEC)
	{
		if (token->type == EXEC || token->type == EXPORT
				|| token->type == EXPORT_AP)
			return (true);
	}
	if (type == NODE)
	{
		if (token->type == PIPE || token->type == AND || token->type == OR)
			return (true);
	}
	return (false);
}
