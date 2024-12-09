#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_shell	t_shell;
typedef enum e_env		t_env;

//ft_env.c
void ft_env(int argc, char **args, char **my_envp);

//ft_cd.c
void	ft_cd(int argc, const char *new_dir, char **my_envp);
char	*ft_getenv(char *env, char **my_envp);

//ft_echo.c
void	ft_echo(char **args);
int		ft_pwd(void);
void	ft_exit(int argc, char **args, t_shell *sh);
int		ft_unset(int argc, char **argv, t_shell *sh);

//ft_export
int		ft_export(int argc, char **argv, t_shell *sh, t_env mode);
int		check_key(char **argv);
void	ft_print_export(char **envp);
int		add_var(char *str, size_t size, char ***my_envp);
void	ft_local_import(t_shell *sh, char *arg);

//manage
bool	ft_isbuiltin(char **args);
void	ft_exec_builtin(char **args, t_shell *sh);
int		ft_argslen(char **args);
int		ft_isjustbuiltin(void *node, t_shell *sh);

#endif //BUILTINS_H