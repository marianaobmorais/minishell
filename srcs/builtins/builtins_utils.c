#include "../../includes/minishell.h"

int	ft_argslen(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (i);
	while (args[i])
		i++;
	return (i);
}

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
	// TODO builtin nao esta sendo salvo as alteracoes
	if (ft_strncmp("cd", args[0], ft_strlen(args[0])) == 0)
		ft_cd(ft_argslen(args), args[1], my_envp);
	else if (ft_strncmp("pwd", args[0], ft_strlen(args[0])) == 0)
		ft_pwd();
	else if (ft_strncmp("export", args[0], ft_strlen(args[0])) == 0)
		ft_export(ft_argslen(args), args, my_envp);
	else if (ft_strncmp("unset", args[0], ft_strlen(args[0])) == 0)
		ft_unset(ft_argslen(args), args, my_envp);
	else if (ft_strncmp("exit", args[0], ft_strlen(args[0])) == 0)
		ft_exit(args);
	else if (ft_strncmp("echo", args[0], ft_strlen(args[0])) == 0)
		ft_echo(args);
	else if (ft_strncmp("env", args[0], ft_strlen(args[0])) == 0)
		ft_env(my_envp);
}
