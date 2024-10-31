#ifndef BUILTINS_H
#define BUILTINS_H

void	ft_env(char **my_envp);
void	ft_cd(int argc, const char *new_dir, char **my_envp);
void	ft_echo(char **args);

int	ft_pwd(void);
int	ft_export(int argc, char **argv, char ***my_envp);
int	ft_unset(int argc, char **argv, char ***my_envp);
char	**create_env(char **envp);
void	ft_print_export(char **envp);

#endif //BUILTINS_H