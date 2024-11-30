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
		$(SRCS_DIR)/tokenize/ft_process_input.c \
		$(SRCS_DIR)/tokenize/ft_charjoin.c \
		$(SRCS_DIR)/tokenize/ft_isspace.c \
		$(SRCS_DIR)/tokenize/ft_validate_syntax.c \
		$(SRCS_DIR)/tokenize/ft_validate_syntax_utils.c \
		$(SRCS_DIR)/tokenize/ft_create_token_list.c \
		$(SRCS_DIR)/tokenize/ft_create_token_list_utils.c \
		$(SRCS_DIR)/tokenize/ft_process_token_list.c \
		$(SRCS_DIR)/tokenize/ft_process_token_list_utils.c \
		$(SRCS_DIR)/tokenize/ft_find_next_quote.c \
		$(SRCS_DIR)/tokenize/ft_build_tree.c \
		$(SRCS_DIR)/tokenize/ft_build_branch.c \
		$(SRCS_DIR)/tokenize/ft_build_branch_utils.c \
		$(SRCS_DIR)/tokenize/ft_is_token_type.c \
		$(SRCS_DIR)/tokenize/ft_free_tree.c \
		$(SRCS_DIR)/builtins/ft_env.c \
		$(SRCS_DIR)/builtins/ft_cd.c \
		$(SRCS_DIR)/builtins/ft_echo.c \
		$(SRCS_DIR)/builtins/ft_pwd.c \
		$(SRCS_DIR)/builtins/ft_export.c \
		$(SRCS_DIR)/builtins/ft_export_utils.c \
		$(SRCS_DIR)/builtins/ft_unset.c \
		$(SRCS_DIR)/builtins/ft_exit.c \

#BONUS_SRCS = $(BONUS_DIR)/

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

RM = rm -f

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --supressions=external_func_leaks.supp


$(NAME): $(LIBFT) $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME) -lreadline

#bonus: $(BONUS_NAME)

#$(BONUS_NAME): $(LIBFT) $(BONUS_SRCS)
#	$(CC) $(CFLAGS) $(BONUS_SRCS) $(LIBFT) -o $(BONUS_NAME)

all: $(NAME) #bonus

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME) $(LIBFT)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean
	$(MAKE) all

test: $(NAME)
	$(VALGRIND) ./$(NAME)

.PHONY: all clean fclean re #bonus