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
	const char	*bcmd[8];
	int			i;

	i = 0;
	bcmd[0] = "cd";
	bcmd[1] = "pwd";
	bcmd[2] = "export";
	bcmd[3] = "unset";
	bcmd[4] = "exit";
	bcmd[5] = "echo";
	bcmd[6] = "env";
	bcmd[7] = NULL;
	while (bcmd[i])
	{
		if (ft_strncmp(bcmd[i], args[0], ft_strlen(args[0])) == 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

void	ft_exec_builtin(char **args, t_shell *sh)
{
	if (ft_strncmp("cd", args[0], ft_strlen(args[0])) == 0)
		ft_cd(ft_argslen(args), args[1], sh->global);
	else if (ft_strncmp("pwd", args[0], ft_strlen(args[0])) == 0)
		ft_pwd();
	else if (ft_strncmp("export", args[0], ft_strlen(args[0])) == 0)
		ft_export(ft_argslen(args), args, sh, GLOBAL);
	else if (ft_strncmp("unset", args[0], ft_strlen(args[0])) == 0)
		ft_unset(ft_argslen(args), args, sh);
	else if (ft_strncmp("exit", args[0], ft_strlen(args[0])) == 0)
		ft_exit(args);
	else if (ft_strncmp("echo", args[0], ft_strlen(args[0])) == 0)
		ft_echo(args);
	else if (ft_strncmp("env", args[0], ft_strlen(args[0])) == 0)
		ft_env(sh->global);
}

int	ft_isjustbuiltin(void *node, t_shell *sh)
{
	void	*curr_node;
	char	**new_args;
	int		flag;

	flag = 0;
	if (!((t_node *)node)->right)
	{
		curr_node = ((t_node *)node)->left;
		while (!flag && curr_node)
		{
			if (ft_is_node_type(curr_node, REDIR))
				curr_node = ((t_redir *)curr_node)->next;
			else
				flag++;
		}
		if (curr_node)
		{
			if (ft_is_node_type(curr_node, EXEC))
			{
				ft_process_token_list(((t_exec *)curr_node)->args, ft_merge_env(sh));
				new_args = tokentostring(((t_exec *)curr_node)->args);
				if (ft_isbuiltin(new_args) 
					|| ((t_exec *)curr_node)->type == EXPORT
					|| ((t_exec *)curr_node)->type == EXPORT_AP)
					return (ft_free_vector(new_args), TRUE);
			}
		}
	}
	return (FALSE);
}
