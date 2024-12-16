/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:02:01 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 19:02:02 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell_bonus.h"

/**
 * @brief Handles error reporting and status setting for the `cd` command.
 * 
 * This function manages errors that occur during the execution of the 
 * `cd` (change directory) command. It logs the error message to standard 
 * error using `ft_stderror`, optionally including the target directory in 
 * the message. It also sets the shell's exit status to `1`.
 * 
 * @param perror_ An integer indicating whether `perror` or a custom error 
 *        message should be used for error reporting.
 *        - If non-zero, invokes `perror` for error logging.
 * @param message The custom error message to log. This should describe the 
 *        error context or reason.
 * @param new_dir The target directory that caused the error, if applicable. 
 *        If `NULL`, the error message will not include directory information.
 */
static void	ft_error_cd(int perror_, char *message, const char *new_dir)
{
	if (new_dir)
		ft_stderror(perror_, message, new_dir);
	else
		ft_stderror(perror_, message);
	ft_exit_status(1, TRUE, FALSE);
}

/**
 * @brief Updates the PWD and OLDPWD variables in the environment.
 *
 * This function searches through the `my_envp` array and updates the `PWD` 
 * (current working directory) and `OLDPWD` (previous working directory) 
 * environment variables with the new values. It frees the old values before 
 * assigning new ones.
 *
 * @param my_envp A pointer to the array of environment variables to be updated
 * @param cur_pwd The current working directory to set as the new `OLDPWD`.
 */
static void	ft_update_my_envp(char **my_envp, char *cur_pwd)
{
	char	s[1024];
	int		i;

	i = 0;
	while (my_envp[i])
	{
		if (ft_strncmp(my_envp[i], "PWD=", 4) == 0)
		{
			free(my_envp[i]);
			my_envp[i] = ft_strjoin("PWD=", getcwd(s, 1024));
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
 * This function searches the `envp` array for a variable that starts with 
 * the provided `env` string and returns its value (the part after the `=`
 * sign). If the variable is not found, `NULL` is returned.
 *
 * @param env The name of the environment variable to search for.
 * @param envp A pointer to the array of environment variables.
 * @return The value of the environment variable, or `NULL` if not found.
 */
static char	*ft_getenv(char *env, char **envp)
{
	char	*res;

	res = NULL;
	while (*envp)
	{
		if (ft_strncmp(*envp, env, ft_strlen(env)) == 0)
			res = ft_strdup(*envp + ft_strlen(env));
		envp++;
	}
	return (res);
}

/**
 * @brief Changes the current working directory (cd command).
 *
 * This function implements the `cd` command, changing the current working 
 * directory based on the provided path. If no path is given, it changes to 
 * the user's home directory.It also updates the `PWD` and `OLDPWD` environment
 * variables after the directory change. If the path is invalid, an error is
 * printed.
 *
 * @param argc The number of arguments passed to the `cd` command.
 * @param new_dir The directory path to change to.
 * @param envp A pointer to the array of environment variables.
 */
void	ft_cd(int argc, const char *new_dir, char **envp)
{
	char	*cur_pwd;
	char	*home;
	char	s[1024];

	if (argc > 2)
		return (ft_error_cd(FALSE, "cd: too many arguments", NULL));
	cur_pwd = getcwd(s, 100);
	if (argc == 1)
	{
		home = ft_getenv("HOME=", envp);
		if (!home)
			return (ft_error_cd(FALSE, "cd: HOME not set", NULL));
		chdir(home);
		ft_update_my_envp(envp, cur_pwd);
		ft_exit_status(0, TRUE, FALSE);
		return (free(home));
	}
	if (*new_dir)
	{
		if (chdir(new_dir) == -1)
			return (ft_error_cd(TRUE, "cd: %s: ", new_dir));
		ft_update_my_envp(envp, cur_pwd);
	}
	ft_exit_status(0, TRUE, FALSE);
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
