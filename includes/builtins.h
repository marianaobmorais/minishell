#ifndef BUILTINS_H
# define BUILTINS_H

//message
# define ERROR_IDENTIFIER "export: `%s': not a valid identifier"

typedef struct s_shell	t_shell;
typedef enum e_env		t_env;

//ft_env.c
void	ft_env(int argc, char **args, char **envp);

//ft_cd.c
void	ft_cd(int argc, const char *new_dir, char **envp);

//ft_echo.c
void	ft_echo(char **args);
int		ft_pwd(void);
void	ft_exit(int argc, char **args, t_shell *sh);
int		ft_unset(int argc, char **argv, t_shell *sh);
void	ft_delete(char *argv, char ***envp);

//ft_export
int		check_key(char **argv);
int		add_var(char *str, size_t size, char ***envp);
int		ft_export(int argc, char **argv, t_shell *sh, t_env mode);
void	ft_print_export(char **envp);
void	ft_local_import(t_shell *sh, char *arg);
int		ft_limbo_import(t_shell *sh, char *arg);

//builtins_utils.c
bool	ft_isbuiltin(char **args);
void	ft_exec_builtin(char **args, t_shell *sh);
int		ft_argslen(char **args);
int		ft_isjustbuiltin(void *node, t_shell *sh);

#endif //BUILTINS_H