/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:42:55 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/23 16:12:41 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_BONUS_H
# define BUILTINS_BONUS_H

//message
# define ERROR_IDENTIFIER "export: `%s': not a valid identifier"

typedef struct s_shell	t_shell;
typedef enum e_env		t_env;

/* ft_env_bonus.c */

void		ft_env(int argc, char **args, char **envp);

/* ft_cd_bonus.c */

void		ft_cd(int argc, const char *new_dir, char **envp);

/* ft_echo_bonus.c */

int			ft_echo(char **args);

/* ft_pwd_bonus.c */

int			ft_pwd(void);

/* ft_unset_bonus.c */

int			ft_unset(int argc, char **argv, t_shell *sh);

/* ft_exit_bonus.c */

void		ft_exit(int argc, char **args, t_shell *sh);

/* ft_exit_utils_bonus.c */

long long	ft_atoll(const char *nptr, bool *is_sign);

/* ft_export_bonus.c */

int			add_var(char *str, size_t size, char ***envp);
int			concatenate_var(char *str, char ***envp, t_env mode, t_shell *sh);
int			ft_export(int argc, char **argv, t_shell *sh, t_env mode);

/* ft_export_utils_bonus.c */

int			ft_check_keys_argv(char **argv);
int			ft_check_key(char *argv);
void		ft_clear_limbo(char *arg, t_shell *sh);

/* ft_export_ultimate_bonus.c */

int			ft_limbo_import(t_shell *sh, char *arg);
int			ft_local_import(t_shell *sh, char *arg);
int			ft_concat_plus(char *str, t_env mode, t_shell *sh);
char		*ft_chartrim(char *str, char c);
int			delete_var_limbo(char *str, char ***envp, size_t size_env);

/* ft_export_declare_bonus.c */

void		ft_sort_str_tab(char **str, int n);
void		ft_print_export(char **envp);

/* builtins_utils_bonus.c */

bool		ft_isbuiltin(char **args);
void		ft_exec_builtin(char **args, t_shell *sh);
int			ft_argslen(char **args);
int			ft_isjustbuiltin(void *node, t_shell *sh);

#endif //BUILTINS_BONUS_H