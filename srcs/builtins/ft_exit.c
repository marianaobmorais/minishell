#include "../../includes/minishell.h"

static long	arg_convert(char *arg)
{
	int			i;
	long long	num;

	i = 0;
	
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
		{
			ft_stderror(FALSE, "exit: %s: numeric argument required", arg);
			ft_exit_status(2, TRUE, FALSE);
		}
		i++;
	}
	num = ft_atol(arg); //rever isso
	//printf("num %lld\nmax %ld\n long long max %lld", num, LONG_MAX, LLONG_MAX);
	if (num > INT32_MAX || num < INT32_MIN)
	{
		ft_stderror(FALSE, "exit: %s: numeric argument required", arg);
		ft_exit_status(2, TRUE, FALSE);
	}
	return ((long) num);
}
/**
 * @brief Exits the program with a specified exit status and displays an exit
 * message.
 *
 * This function prints "exit" to the standard error output (file descriptor 2)
 * and then calls `ft_exit_status` to set the exit status. If `exit_` is `TRUE`
 * it terminates the program using the current status.
 *
 * @param exit_ If `TRUE`, the function will terminate the program with the 
 * 				current exit status.
 *              If `FALSE`, it only updates the exit status without terminating.
 * @param exit_status Terminate the program with the current exit status.
 */
void	ft_exit(char **args)
{
	unsigned char	exit_status;
	int				i;

	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (i == 2)
		{
			ft_stderror(FALSE, "exit: too many arguments");
			ft_exit_status(1, TRUE, FALSE);
		}
		if (args[i][0] == '\0' || ((args[i][0] == '-'
			|| args[i][0] == '+') && !ft_isdigit(args[i][1])))
		{
			ft_stderror(FALSE, "exit: %s: numeric argument required", args[i]);
			ft_exit_status(2, TRUE, FALSE);
		}
		exit_status = (unsigned char) arg_convert(args[i]);
		i++;
	}
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	ft_exit_status(exit_status, TRUE, TRUE);
}
