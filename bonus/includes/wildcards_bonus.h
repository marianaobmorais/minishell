/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:49:35 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/23 16:49:36 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARDS_BONUS_H
# define WILDCARDS_BONUS_H

/* ft_copy_entry_list_bonus.c */

t_list	**ft_copy_entry_list(void);

/* ft_get_wildcard_list_bonus.c */

t_list	**ft_get_wildcard_list(char *s);
bool	ft_find_middle(char *s, bool prefix);

/* ft_get_wildcard_list_utils_bonus.c */

void	ft_match_prefix(t_list **list, char *s);
void	ft_match_sufix(t_list **list, char *s);

char	*ft_get_middle(char **s, bool prefix);

/* ft_get_wildcard_list_utils2_bonus.c */

int		ft_strncmp_(const char *str1, const char *str2, size_t len);
char	*ft_strnstr_(char **big, const char *little, size_t len);
char	*ft_get_middle(char **s, bool prefix);
bool	ft_handle_middle(char *s, char *entry_name, bool prefix);
void	ft_match_middle(t_list **list, char *s, bool prefix);

/* ft_set_alphabetical_order_bonus.c */

t_list	**ft_set_alphabetical_order(t_list **old);

#endif //WILDCARDS_BONUS_H