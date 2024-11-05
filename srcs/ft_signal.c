#include "../includes/minishell.h"

void	handler_sigint(int sig)
{
	(void)sig;
	printf("\n^C\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	off_control_char(void)
{
	struct termios tty;

	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void	on_control_char(void)
{
	struct termios tty;

	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void	ft_signal_parent(void)
{

	off_control_char();
	signal(SIGINT, &handler_sigint);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	ft_signal_child(void)
{
	on_control_char();
	signal(SIGQUIT, SIG_DFL);
}
