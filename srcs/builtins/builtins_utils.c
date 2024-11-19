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
		ft_exit(0, TRUE); //organizar para receber args
	if (ft_strncmp("echo", args[0], ft_strlen(args[0])) == 0)
		ft_echo(args);
	if (ft_strncmp("env", args[0], ft_strlen(args[0])) == 0)
		ft_env(my_envp);
}
