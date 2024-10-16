#include "../../includes/minishell.h"

static void	ft_update_my_envp(char **my_envp, char *cur_pwd)
{
	char	s[100];
	int	i;

	i = 0;
	while (my_envp[i])
	{
		if (ft_strncmp(my_envp[i], "PWD=", 4) == 0)
		{
			free(my_envp[i]);
			my_envp[i] = ft_strjoin("PWD=", getcwd(s, 100));
		}
		if (ft_strncmp(my_envp[i], "OLDPWD=", 7) == 0)
		{
			free(my_envp[i]);
			my_envp[i] = ft_strjoin("OLDPWD=", cur_pwd);
		}
		i++;
	}
}


void	ft_cd(int argc, const char *new_dir, char **my_envp)
{
	char	*cur_pwd;
	char	s[100];

	if (argc > 2)
	{
		ft_error_handler(); //printf("%s: cd: too many arguments\n", PROMPT); // precisamos printar no STDERR
		return ;
	}
	cur_pwd = getcwd(s, 100);
	if (argc == 1)
	{
		chdir(getenv("HOME"));
		ft_update_my_envp(my_envp, cur_pwd);
		return ;
	}
	if (chdir(new_dir) == -1)
	{
		ft_error_handler(); //printf("%s: cd: %s: no such file or directory\n", new_dir, PROMPT); // precisamos printar no STDERR
		return ;
	}
	ft_update_my_envp(my_envp, cur_pwd);
}

