#include "../includes/minishell.h"

void	ft_prompt(t_shell *sh)
{
	char	*prompt;
	char	*prog_name_initial;
	char	*prog_name_final;
	char	current_path[OPENFD_MAX];

	getcwd(current_path, OPENFD_MAX);
	prog_name_initial = "bashinho [";
	prog_name_final = "] $ ";
	if (current_path != NULL)
		prompt = ft_strjoin(prog_name_initial, current_path);
	else
		prompt = ft_strjoin(prog_name_initial, "banana");
	prompt = ft_strjoin(prompt, "banana");
	//BOLD  "[" current_path "]" RESET;
	sh->prompt = prompt;
}