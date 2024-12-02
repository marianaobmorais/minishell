NAME = minishell
#NAME_BONUS = minishell_bonus

SRCS_DIR = ./srcs
#BONUS_DIR = ./bonus

LIBFT = ./libft/libft.a
LIBFT_DIR = ./libft

SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/ft_error_handler.c \
		$(SRCS_DIR)/ft_get_my_envp.c \
		$(SRCS_DIR)/ft_free_utils.c \
		$(SRCS_DIR)/ft_cli.c \
		$(SRCS_DIR)/ft_exit_status.c \
		$(SRCS_DIR)/ft_signal.c \
		$(SRCS_DIR)/execution/ft_heredoc.c \
		$(SRCS_DIR)/execution/ft_exec.c \
		$(SRCS_DIR)/tokenize/ft_process_input.c \
		$(SRCS_DIR)/tokenize/ft_charjoin.c \
		$(SRCS_DIR)/tokenize/ft_isspace.c \
		$(SRCS_DIR)/tokenize/ft_validate_syntax.c \
		$(SRCS_DIR)/tokenize/ft_validate_syntax_utils.c \
		$(SRCS_DIR)/tokenize/ft_validate_parentheses.c \
		$(SRCS_DIR)/tokenize/ft_validate_parentheses_utils.c \
		$(SRCS_DIR)/tokenize/ft_create_token_list.c \
		$(SRCS_DIR)/tokenize/ft_create_token_list_utils.c \
		$(SRCS_DIR)/tokenize/ft_process_token_list.c \
		$(SRCS_DIR)/tokenize/ft_process_token_list_utils.c \
		$(SRCS_DIR)/tokenize/ft_get_wildcard_list.c \
		$(SRCS_DIR)/tokenize/ft_find_next_quote.c \
		$(SRCS_DIR)/tokenize/ft_build_root.c \
		$(SRCS_DIR)/tokenize/ft_build_tree.c \
		$(SRCS_DIR)/tokenize/ft_build_branch.c \
		$(SRCS_DIR)/tokenize/ft_build_branch_utils.c \
		$(SRCS_DIR)/tokenize/ft_build_branch_utils2.c \
		$(SRCS_DIR)/tokenize/ft_free_tree.c \
		$(SRCS_DIR)/tokenize/ft_is_token_type.c \
		$(SRCS_DIR)/builtins/ft_env.c \
		$(SRCS_DIR)/builtins/ft_cd.c \
		$(SRCS_DIR)/builtins/ft_echo.c \
		$(SRCS_DIR)/builtins/ft_pwd.c \
		$(SRCS_DIR)/builtins/ft_export.c \
		$(SRCS_DIR)/builtins/ft_export_utils.c \
		$(SRCS_DIR)/builtins/ft_unset.c \
		$(SRCS_DIR)/builtins/ft_exit.c \
		#$(SRCS_DIR)/builtins/builtins_utils.c \
		$(SRCS_DIR)/execution/ft_launcher.c \
		$(SRCS_DIR)/execution/ft_redir.c \

#BONUS_SRCS = $(BONUS_DIR)/

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

RM = rm -f

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --supressions=external_func_leaks.supp


$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

#bonus: $(BONUS_NAME)

#$(BONUS_NAME): $(LIBFT) $(BONUS_SRCS)
#	$(CC) $(CFLAGS) $(BONUS_SRCS) $(LIBFT) -o $(BONUS_NAME)

all: $(NAME) #bonus

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME) $(LIBFT)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean
	$(MAKE) all

test: $(NAME)
	$(VALGRIND) ./$(NAME)

.PHONY: all clean fclean re #bonus