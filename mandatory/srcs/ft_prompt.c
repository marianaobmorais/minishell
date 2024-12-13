#include "../includes/minishell.h"

void	ft_prompt(t_shell *sh)
{
	char	*prompt;
	char	current_path[OPENFD_MAX];

	getcwd(current_path, OPENFD_MAX);
	prompt = ft_strjoin(PROMPT_1, current_path);
	prompt = merge(prompt, PROMPT_2);
	sh->prompt = prompt;
}
