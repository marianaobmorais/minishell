#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_env	t_env;

//ft_env.c
void	ft_env(char **my_envp);

//ft_cd.c
void	ft_cd(int argc, const char *new_dir, char **my_envp);
char	*ft_getenv(char *env, char **my_envp);

//ft_echo.c
void	ft_echo(char **args);
int		ft_pwd(void);
void	ft_exit(char **args);
int		ft_unset(int argc, char **argv, char ***my_envp);
int		ft_export(int argc, char **argv, char ***my_envp);
void	ft_print_export(char **envp);

//manage
bool	ft_isbuiltin(char **args);
void	ft_exec_builtin(char **args, t_env *env);
int		ft_argslen(char **args);
int		ft_isjustbuiltin(void *node);

#endif //BUILTINS_H