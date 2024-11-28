#include "../../includes/minishell.h"

/**
 * @brief Updates the PWD and OLDPWD variables in the environment.
 *
 * This function searches through the `my_envp` array and updates the `PWD` 
 * (current working directory) and `OLDPWD` (previous working directory) 
 * environment variables with the new values. It frees the old values before 
 * assigning new ones.
 *
 * @param my_envp A pointer to the array of environment variables to be updated.
 * @param cur_pwd The current working directory to set as the new `OLDPWD`.
 */
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
			my_envp[i] = ft_strjoin("PWD=", getcwd(s, 1024)); //this function will get the pwd from my_envp or from system's envp?
		}
		if (ft_strncmp(my_envp[i], "OLDPWD=", 7) == 0)
		{
			free(my_envp[i]);
			my_envp[i] = ft_strjoin("OLDPWD=", cur_pwd);
		}
		i++;
	}
}

/**
 * @brief Retrieves the value of an environment variable.
 *
 * This function searches the `my_envp` array for a variable that starts with 
 * the provided `env` string and returns its value (the part after the `=` sign).
 * If the variable is not found, `NULL` is returned.
 *
 * @param env The name of the environment variable to search for.
 * @param my_envp A pointer to the array of environment variables.
 * @return The value of the environment variable, or `NULL` if not found.
 */
char	*ft_getenv(char *env, char **my_envp)
{
	char	*res;

	res = NULL;
	while (*my_envp)
	{
		if (ft_strncmp(*my_envp, env, ft_strlen(env)) == 0)
			res = ft_strdup(*my_envp + ft_strlen(env));
		my_envp++;
	}
	return (res);
}

/**
 * @brief Changes the current working directory (cd command).
 *
 * This function implements the `cd` command, changing the current working 
 * directory based on the provided path. If no path is given, it changes to 
 * the user's home directory. It also updates the `PWD` and `OLDPWD` environment 
 * variables after the directory change. If the path is invalid, an error is printed.
 *
 * @param argc The number of arguments passed to the `cd` command.
 * @param new_dir The directory path to change to.
 * @param my_envp A pointer to the array of environment variables.
 */
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
	cur_pwd = getcwd(s, 100); //this function will get the pwd from my_envp or from system's envp?
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
	if (!((ft_isalpha(new_dir[0])) || new_dir[0] == '/' || new_dir[0] == '.')) //If I input &, chdir() works // need to test more inside our on minishell
	{
		printf("%s: cd: %s: invalid option\n", PROG_NAME, new_dir); //ft_error_handler();
		return ;
	}
	if (chdir(new_dir) == -1)
	{
		//printf("%s: cd: %s: no such file or directory\n", PROG_NAME, new_dir); //ft_error_handler();
		ft_stderror(TRUE, "%s: ", new_dir);
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
