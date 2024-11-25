#include "../../includes/minishell.h"

int	isvalid(char *pathname, char **args)
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

char	**tokentostring(t_list **args)
{
	char	**new_args;
	t_list	*curr_list;
	int		size;
	int		i;

	curr_list = *args;
	size = ft_lstsize(curr_list);
	i = 0;
	new_args = (char **) malloc((size + 1) * sizeof(char *));
	if (!new_args)
		ft_error_handler();
	while (curr_list)
	{
		new_args[i] = ft_strdup(((t_token *) (curr_list)->content)->value);
		curr_list = (curr_list)->next;
		i++;
	}
	new_args[i] = NULL;
	return (new_args);
}

void	ft_exec(t_list **args, t_env *env)
{
	char	*pathname;
	char	**new_args;

	pathname = NULL;
	ft_process_token_list(args, *env->global); //transformar o t_list em char **
	new_args = tokentostring(args);
	if (ft_isbuiltin(new_args))
		ft_exec_builtin(new_args, env);
	else
	{
		pathname = ft_findpath(*(env)->global, new_args);
		if (!pathname)
		{
			ft_stderror(FALSE, "%s: command not found", new_args[0]);
			ft_exit_status(127, TRUE, TRUE);
		}
		if (execve(pathname, new_args, *(env)->global) == -1)
		{
			ft_stderror(TRUE, "%s:", new_args[0]);
			ft_exit_status(1, TRUE, TRUE);
		}
		free(pathname);
		ft_free_vector(new_args);
	}
}
