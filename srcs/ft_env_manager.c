#include "../includes/minishell.h"

/**
 * @brief Merges global and local environment variables into a single array.
 *
 * Combines the environment variables stored in `sh->global` and `sh->local`
 * into a newly allocated array. Each variable is duplicated, and memory
 * allocation is handled. The resulting array ends with a NULL pointer.
 *
 * @param sh A pointer to the shell structure containing the `global` and
 *           `local` environment variable arrays.
 *
 * @return A new array containing all environment variables, or NULL if
 *         a memory allocation error occurs.
 */
char	**ft_merge_env(t_shell *sh)
{
	int		size;
	int		i;
	int		z;
	char	**envp;

	z = 0;
	i = 0;
	size = ft_argslen(sh->local) + ft_argslen(sh->global);
	envp = (char **) malloc((size + 1) * sizeof(char *));
	if (!envp)
		return (ft_exit_status(1, TRUE, FALSE), ft_stderror(TRUE, ""), NULL);
	while ((sh->global)[i])
	{
		envp[i] = ft_strdup((sh->global)[i]);
		i++;
	}
	if (sh->local)
	{
		while ((sh->local)[z])
		{
			envp[i++] = ft_strdup((sh->local)[z++]);
		}
	}
	envp[i] = NULL;
	return (envp);
}

/**
 * @brief Creates a duplicate of the environment variables array.
 *
 * Allocates memory for a new array and copies each string from the
 * input `envp` array into it. Each string is duplicated, and the
 * resulting array is NULL-terminated.
 *
 * @param envp The original environment variables array to be duplicated.
 *
 * @return A new array containing duplicates of all strings in `envp`, or
 *         NULL if a memory allocation error occurs.
 */
char	**ft_get_my_envp(char **envp)
{
	char	**my_envp;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	my_envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!my_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		my_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	my_envp[i] = NULL;
	return (my_envp);
}
