/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianamorais <marianamorais@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:49:35 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/24 16:02:36 by marianamora      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARDS_BONUS_H
# define WILDCARDS_BONUS_H

/* ft_get_wildcard_list_bonus.c */

t_list	**ft_get_wildcard_list(char *s);
bool	ft_find_middle(char *s);

/* ft_get_wildcard_list_utils_bonus.c */

bool	ft_find_sufix(char *s);
bool	ft_match_prefix(char **s, char **entry_name);
bool	ft_match_sufix(char **s, char **entry_name);

/* ft_get_wildcard_list_utils2_bonus.c */

int		ft_strncmp_(const char *str1, const char *str2, size_t len);
char	*ft_strnstr_(char **big, const char *little, size_t len);
bool	ft_match_middle(char **s, char **entry_name);

/* ft_set_alphabetical_order_bonus.c */

t_list	**ft_set_alphabetical_order(t_list **old);


#endif //WILDCARDS_BONUS_H