# include "../../includes/minishell.h"

int	ft_pwd(void)
{
	char	current_path[1024];

	if (getcwd(current_path, 1024) != NULL)
		printf("%s", current_path);
	else
	 	return (-1);
	return (0);
}
