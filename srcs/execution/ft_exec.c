#include "../../includes/minishell.h"

int	isvalid(char *pathname, char **args)
{
	struct stat	file;
	
	if (stat(pathname, &file) == -1)
	{
		ft_stderror(TRUE, "%s: ", args[0]);
		ft_exit_status(126, TRUE, TRUE);
		return (-1);
	}
	if (S_ISDIR(file.st_mode) != 0)
	{
		ft_stderror(FALSE, "%s: Is a directory", args[0]);
		ft_exit_status(126, TRUE, TRUE);
		return (-1);
	}
	if (access(pathname, X_OK) == -1)
	{
		return (-1);
	}
	return (0);
}

static char	*merge(char *s1, char *s2)
{
	char	*merge;

	merge = ft_strjoin(s1, s2);
	free(s1);
	s1 = NULL;
	return (merge);
}

static int	ft_free_paths(char **paths, int i)
{
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (0);
}

static char	*ft_findpath(char **envp, char **cmds)
{
	char	**paths;
	char	*pathname;
	int		i;

	i = 0;
	paths = NULL;
	if (access(cmds[0], F_OK) == 0 && isvalid(cmds[0], cmds) == 0)
		return (ft_strdup(cmds[0]));
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (envp[i])
		paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		pathname = merge(merge(paths[i], "/"), cmds[0]);
		if (access(pathname, F_OK) == 0 && isvalid(pathname, cmds) == 0)
			return (ft_free_paths(paths, i), pathname);
		free(pathname);
		i++;
	}
	free(paths);
	return (NULL);
}

void	ft_exec(t_list **args, t_shell *sh)
{
	char	*pathname;
	char	**new_args;

	pathname = NULL;
	ft_process_token_list(args, sh->global); //ft_merge_env
	new_args = ft_split_argv(tokentostring(args));
	if (ft_isbuiltin(new_args))
		ft_exec_builtin(new_args, sh);
	else
	{
		pathname = ft_findpath(sh->global, new_args); //verificar se precisa de ft_merge_env
		if (!pathname)
		{
			ft_stderror(FALSE, "%s: command not found", new_args[0]);
			ft_exit_status(127, TRUE, TRUE);
		}
		if (execve(pathname, new_args, sh->global) == -1) //ft_merge_env
		{
			ft_stderror(TRUE, "%s:", new_args[0]);
			ft_exit_status(1, TRUE, TRUE);
		}
		free(pathname);
		ft_free_vector(new_args);
	}
	ft_exit_status(0, TRUE, TRUE);
}
