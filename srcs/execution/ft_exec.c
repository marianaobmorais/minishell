#include "../../includes/minishell.h"

static int	isvalid(char *pathname, char **args)
{
	if (access(pathname, X_OK) == -1)
	{
		ft_stderror(TRUE, "%s: ", args[0]);
		ft_exit_status(126, TRUE, TRUE);
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

void	ft_exec(char **args, t_env *env)
{
	char *pathname;

	pathname = NULL;
	//if (ft_isbuiltin(args))
	//	ft_exec_builtin(args, env);
	//else
	//{
		pathname = ft_findpath(*(env)->global, args);
		if (!pathname)
		{
			ft_stderror(FALSE, "%s: command not found", args[0]);
			ft_exit_status(127, TRUE, TRUE);
		}
		if (execve(pathname, args, *(env)->global) == -1)
			ft_stderror(TRUE, "%s:", args[0]);
		free(pathname);
	//}
}
