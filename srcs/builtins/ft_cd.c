#include "../../includes/minishell.h"

static void	ft_update_my_envp(char **my_envp, char *cur_pwd)
{
	char	s[1024];
	int	i;

	i = 0;
	while (my_envp[i])
	{
		if (ft_strncmp(my_envp[i], "PWD=", 4) == 0)
		{
			free(my_envp[i]);
			my_envp[i] = ft_strjoin("PWD=", getcwd(s, 1024)); // essa funcao vai pegar o pwd do my_envp ou do envp?
		}
		if (ft_strncmp(my_envp[i], "OLDPWD=", 7) == 0)
		{
			free(my_envp[i]);
			my_envp[i] = ft_strjoin("OLDPWD=", cur_pwd);
		}
		i++;
	}
}


char	*ft_getenv(char *env, char **my_envp)
{
	char	*res;
	int		i;

	res = NULL;
	i = 0;
	while (*my_envp)
	{
		if (ft_strncmp(*my_envp, env, ft_strlen(env)) == 0)
			res = ft_strdup(*my_envp + ft_strlen(env));
		my_envp++;
	}
	return (res);
}

void	ft_cd(int argc, const char *new_dir, char **my_envp)
{
	char	*cur_pwd;
	char	*home;
	char	s[1024];

	if (argc > 2)
	{
		ft_error_handler(); //printf("%s: cd: too many arguments\n", PROMPT); // precisamos printar no STDERR
		return ;
	}
	cur_pwd = getcwd(s, 100);
	if (argc == 1)
	{
		home = ft_getenv("HOME=", my_envp); // not sure if this is necessary //home = getenv("HOME");
		if (!home)
		{
			ft_error_handler(); //printf("%s: cd: HOME not set \n", PROMPT); // precisamos printar no STDERR
			return (free(home));
		}
		chdir(home);
		ft_update_my_envp(my_envp, cur_pwd);
		return (free(home));
	}
	if (chdir(new_dir) == -1)
	{
		ft_error_handler(); //printf("%s: cd: %s: no such file or directory\n", PROMPT, new_dir); // precisamos printar no STDERR
		return ;
	}
	ft_update_my_envp(my_envp, cur_pwd);
}


