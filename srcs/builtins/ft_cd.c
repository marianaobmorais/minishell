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
		printf("%s: cd: too many arguments\n", PROG_NAME); //ft_error_handler();
		return ;
	}
	cur_pwd = getcwd(s, 100); // essa funcao vai pegar o pwd do my_envp ou do envp?
	if (argc == 1)
	{
		home = ft_getenv("HOME=", my_envp); // not sure if this function is necessary //home = getenv("HOME"); essa funcao vai pegar o pwd do my_envp ou do envp?
		if (!home)
		{
			printf("%s: cd: HOME not set \n", PROG_NAME); //ft_error_handler();
			return (free(home));
		}
		chdir(home);
		ft_update_my_envp(my_envp, cur_pwd);
		return (free(home));
	}
	//from subject: cd with only a relative or absolute path
	if (!((ft_isalpha(new_dir[0])) || new_dir[0] == '/' || new_dir[0] == '.')) //se eu coloco &, o chdir funciona // preciso testar mais dentro no bashinho
	{
		printf("%s: cd: %s: invalid option\n", PROG_NAME, new_dir); //ft_error_handler();
		return ;
	}
	if (chdir(new_dir) == -1)
	{
		printf("%s: cd: %s: no such file or directory\n", PROG_NAME, new_dir); //ft_error_handler();
		return ;
	}
	ft_update_my_envp(my_envp, cur_pwd);
}

/* int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char **my_envp;

	// if (argc != 1)
	// {
	// 	ft_error_handler();
	// 	return (1); // conferir numero de saida
	// }
	my_envp = ft_get_my_envp(envp);
	if (!my_envp)
		return (1);

	int	i = 0;
	while (my_envp[i])
	{
		printf("%s\n", my_envp[i]);
		i++;
	}

	ft_cd(argc, argv[1], my_envp);

	printf("\n\n");
	i = 0;
	while (my_envp[i])
	{
		printf("%s\n", my_envp[i]);
		i++;
	}

	ft_free_vector(my_envp);
	return (0);
} */