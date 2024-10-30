#ifndef BUILTINS_H
#define BUILTINS_H

int	ft_pwd(void);
int	ft_export(int argc, char **argv, char ***my_envp);
int	ft_unset(int argc, char **argv, char ***my_envp);
char	**create_env(char **envp);
void	ft_print_export(char **envp);

#endif //BUILTINS_H