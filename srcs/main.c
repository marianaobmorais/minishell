#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*sh;
	char	*error_msg;

	(void) argv;
	error_msg = "arguments are not accepted when initializing the program";
	if (argc != 1)
		return (ft_stderror(FALSE, error_msg), 1);
	sh = ft_init_sh(envp);
	if (!sh)
		return (1);
	ft_cli(sh);
	ft_free_sh(sh);
	return (0);
}
