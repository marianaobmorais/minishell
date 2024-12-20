/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joneves- <joneves-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:41:06 by joneves-          #+#    #+#             */
/*   Updated: 2024/12/17 15:41:07 by joneves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles errors related to memory allocation.
 *
 * Logs the specified error message to the standard error and sets the
 * exit status to 1. Used for critical memory allocation failures.
 *
 * @param message A string describing the context of the memory allocation error.
 */
void	ft_error_malloc(char *message)
{
	ft_stderror(TRUE, message);
	ft_exit_status(1, TRUE, FALSE);
}

/**
 * @brief Converts an integer character to a string.
 *
 * This function takes an integer character and converts it to a string. If the
 * character is 0, it prints the character to the standard output and returns 
 * NULL. Otherwise, it allocates memory for a 2-character string, assigns the 
 * character to the first position, and null-terminates the string.
 *
 * @param c The integer character to be converted.
 * @return char* Pointer to the newly allocated string, or NULL if the 
 *         character is 0.
 */
static char	*ctoa(int c)
{
	char	*str;

	if (c == 0)
	{
		ft_putchar_fd(c, 1);
		return (NULL);
	}
	str = (char *) malloc(2 * sizeof(char));
	str[0] = (char) c;
	str[1] = '\0';
	return (str);
}

/**
 * @brief Formats a single argument based on the specified format character.
 *
 * This function processes a variable argument list (`va_list`) and formats 
 * a single argument based on the provided format character. Currently, 
 * it supports the format specifier 'd' and 'i' for numbers, 's' for strings. 
 * If the argument is  `NULL`, it returns a copy of the string "(null)".
 *
 * @param args The list of arguments to format.
 * @param fmt The format specifier character (e.g., 's' for strings).
 * @return A dynamically allocated formatted string. If allocation fails,
 *         it returns "(null)" instead.
 */
static char	*ft_format(va_list args, const char fmt)
{
	char	*specifier;

	specifier = NULL;
	if (fmt == 's')
	{
		specifier = ft_strdup(va_arg(args, char *));
		if (!specifier)
			return (ft_strdup("(null)"));
	}
	else if (fmt == 'd' || fmt == 'i')
		specifier = ft_itoa(va_arg(args, int));
	else if (fmt == 'c')
		specifier = ctoa(va_arg(args, int));
	return (specifier);
}

/**
 * @brief Writes a string of a specified length to a given file descriptor.
 *
 * If the string is NULL, it writes the string "(null)" with a length of 6.
 * Otherwise, writes the first `len` characters of the string `s` to the file
 * descriptor `fd`.
 *
 * @param s The string to be written. If NULL, writes "(null)" instead.
 * @param fd The file descriptor where the string will be written.
 * @param len The number of characters to write from the string.
 */
void	ft_putstr_fd_len(char *s, int fd, int len)
{
	if (s == NULL)
		return (ft_putstr_fd_len("(null)", fd, 6));
	write(fd, s, len);
}

/**
 * @brief Main command-line interface loop for processing user commands.
 *
 * This function initializes and enters an infinite loop where it prompts the
 * user for input, processes the input, and handles command execution. It sets
 * up signal handling, reads user input from the prompt, and checks for valid
 * commands to add to history and execute. If the input is empty (EOF), it 
 * exits the program by printing "exit".
 *
 * @param sh A pointer to the shell struct containing the current shell state.
 */
void	ft_stderror(int perror_, const char *str, ...)
{
	va_list	args;
	char	*fmt_specifier;
	char	*all;

	va_start(args, str);
	all = merge(ft_strdup(""), PROG_NAME_ERROR);
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			fmt_specifier = ft_format(args, *str);
			all = merge(all, fmt_specifier);
			free(fmt_specifier);
		}
		else
			all = ft_charjoin(all, *str);
		str++;
	}
	if (perror_ == TRUE)
		return (ft_putstr_fd_len(all, 2, ft_strlen(all)), perror(""), \
			free(all), va_end(args));
	all = merge(all, "\n");
	return (ft_putstr_fd_len(all, 2, ft_strlen(all)), free(all), va_end(args));
}
