#include "../../includes/minishell.h"

bool	ft_isbuiltin(char **args)
{
	char	*bcmd[8] = {"cd", "pwd", "export", "unset", "exit", "echo", "env", NULL};
	int	i = 0;

	while (bcmd[i])
	{
		if (ft_strncmp(bcmd[i], args[0], ft_strlen(args[0])) == 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

void	ft_exec_builtin(char **args, char **my_envp)
{
	if (ft_strncmp("cd", args[0], ft_strlen(args[0])) == 0)
		//ft_cd();
	if (ft_strncmp("pwd", args[0], ft_strlen(args[0])) == 0)
		ft_pwd();
	if (ft_strncmp("export", args[0], ft_strlen(args[0])) == 0)
		//ft_export();
	if (ft_strncmp("unset", args[0], ft_strlen(args[0])) == 0)
		//ft_unset();
	if (ft_strncmp("exit", args[0], ft_strlen(args[0])) == 0)
		//ft_exit();
	if (ft_strncmp("echo", args[0], ft_strlen(args[0])) == 0)
		ft_echo(args);
	if (ft_strncmp("env", args[0], ft_strlen(args[0])) == 0)
		ft_env(my_envp);
}

char	*merge(char *s1, char *s2)
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
	if (access(cmds[0], F_OK) == 0 && access(cmds[0], X_OK) == 0)
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
		if (access(pathname, F_OK) == 0 && access(pathname, X_OK) == 0)
			return (ft_free_paths(paths, i), pathname);
		free(pathname);
		i++;
	}
	free(paths);
	return (NULL);
}

void	ft_exec(char **args, char **my_envp)
{
	char *pathname;

	if (ft_isbuiltin(args))
		ft_exec_builtin(args, my_envp);
	else
	{
		pathname = ft_findpath(my_envp, args);
		if (execve(pathname, args, my_envp) == -1)
			ft_error_handler();
	}
}
