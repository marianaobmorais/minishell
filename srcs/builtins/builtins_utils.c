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

void	ft_exec_builtin(char **args, t_shell *sh)
{
	if (ft_strncmp("cd", args[0], ft_strlen(args[0])) == 0)
		ft_cd(ft_argslen(args), args[1], sh->global);
	else if (ft_strncmp("pwd", args[0], ft_strlen(args[0])) == 0)
		ft_pwd();
	else if (ft_strncmp("export", args[0], ft_strlen(args[0])) == 0)
		ft_export(ft_argslen(args), args, sh, GLOBAL); //modificar receber global e local
	else if (ft_strncmp("unset", args[0], ft_strlen(args[0])) == 0)
		ft_unset(ft_argslen(args), args, &sh->global); //receber global e local
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
	if (!((t_pipe *)node)->right)
	{
		curr_node = ((t_pipe *)node)->left;
		while (!flag && curr_node)
		{
			if (((t_redir *)curr_node)->type == OUTFILE 
				|| ((t_redir *)curr_node)->type == INFILE
				|| ((t_redir *)curr_node)->type == APPEND
				|| ((t_redir *)curr_node)->type == HEREDOC)
				curr_node = ((t_redir *)curr_node)->next;
			else
				flag++;
		}
		if (curr_node)
		{
			if (((t_exec *)curr_node)->type == EXEC)
			{
				ft_process_token_list(((t_exec *)curr_node)->args, sh->global);//ft_merge_env
				new_args = tokentostring(((t_exec *)curr_node)->args);
				if (ft_isbuiltin(new_args))
					return (ft_free_vector(new_args), TRUE);
			}
		}
	}
	return (FALSE);
}
