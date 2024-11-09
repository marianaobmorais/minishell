#include "../includes/minishell.h"

void	sig_parent_handler(int sig)
{
	write(1, "\n", 1);
	ft_exit_status(130, TRUE, FALSE);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void)sig;
}

void	sig_child_handler(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	if (sig == SIGQUIT)
		ft_putendl_fd("Quit (core dumped)", 1);
}

void	sig_heredoc_handler(int sig)
{
	(void)sig;
	//signal(SIGINT, handler_sigint);
	printf("HEREDOC\n");
}

void	ft_signal(int type)
{
	signal(SIGTSTP, SIG_IGN);
	if (type == PARENT)
	{
		signal(SIGINT, sig_parent_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	if (type == HEREDOC)
	{
		signal(SIGQUIT, SIG_IGN);
		//configurar handler
	}
	if (type == DEFAULT)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	if (type == CHILD)
	{
		signal(SIGINT, sig_child_handler);
		signal(SIGQUIT, sig_child_handler);
	}
}
