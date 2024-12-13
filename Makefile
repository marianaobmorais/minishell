NAME = minishell
BONUS_NAME = minishell_bonus

SRCS_DIR = ./mandatory/srcs
BONUS_DIR = ./bonus/srcs

LIBFT = ./libft/libft.a
LIBFT_DIR = ./libft

SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/error_handler.c \
		$(SRCS_DIR)/ft_env_manager.c \
		$(SRCS_DIR)/ft_free_utils.c \
		$(SRCS_DIR)/ft_cli.c \
		$(SRCS_DIR)/ft_exit_status.c \
		$(SRCS_DIR)/ft_signal.c \
		$(SRCS_DIR)/ft_copy_list.c \
		$(SRCS_DIR)/ft_prompt.c \
		$(SRCS_DIR)/execution/ft_heredoc.c \
		$(SRCS_DIR)/execution/ft_heredoc_utils.c \
		$(SRCS_DIR)/execution/ft_launcher.c \
		$(SRCS_DIR)/execution/ft_launcher_utils.c \
		$(SRCS_DIR)/execution/ft_exec.c \
		$(SRCS_DIR)/execution/ft_redir.c \
		$(SRCS_DIR)/execution/ft_exec_utils.c \
		$(SRCS_DIR)/tokenize/ft_process_input.c \
		$(SRCS_DIR)/tokenize/ft_charjoin.c \
		$(SRCS_DIR)/tokenize/ft_isspace.c \
		$(SRCS_DIR)/tokenize/ft_validate_syntax.c \
		$(SRCS_DIR)/tokenize/ft_validate_syntax_utils.c \
		$(SRCS_DIR)/tokenize/ft_create_token_list.c \
		$(SRCS_DIR)/tokenize/ft_create_token_list_utils.c \
		$(SRCS_DIR)/tokenize/ft_create_token_list_utils2.c \
		$(SRCS_DIR)/tokenize/ft_process_token_list.c \
		$(SRCS_DIR)/tokenize/ft_process_token_list_utils.c \
		$(SRCS_DIR)/tokenize/ft_find_next_quote.c \
		$(SRCS_DIR)/tokenize/ft_build_tree.c \
		$(SRCS_DIR)/tokenize/ft_build_branch.c \
		$(SRCS_DIR)/tokenize/ft_build_branch_utils.c \
		$(SRCS_DIR)/tokenize/ft_free_tree.c \
		$(SRCS_DIR)/tokenize/ft_is_token_type.c \
		$(SRCS_DIR)/builtins/ft_env.c \
		$(SRCS_DIR)/builtins/ft_cd.c \
		$(SRCS_DIR)/builtins/ft_echo.c \
		$(SRCS_DIR)/builtins/ft_pwd.c \
		$(SRCS_DIR)/builtins/ft_export.c \
		$(SRCS_DIR)/builtins/ft_export_utils.c \
		$(SRCS_DIR)/builtins/ft_export_ultimate.c \
		$(SRCS_DIR)/builtins/ft_unset.c \
		$(SRCS_DIR)/builtins/ft_exit.c \
		$(SRCS_DIR)/builtins/ft_exit_utils.c \
		$(SRCS_DIR)/builtins/builtins_utils.c \

BONUS_SRCS = $(BONUS_DIR)/main_bonus.c \
		$(BONUS_DIR)/ft_error_handler_bonus.c \
		$(BONUS_DIR)/ft_env_manager_bonus.c \
		$(BONUS_DIR)/ft_free_utils_bonus.c \
		$(BONUS_DIR)/ft_cli_bonus.c \
		$(BONUS_DIR)/ft_exit_status_bonus.c \
		$(BONUS_DIR)/ft_signal_bonus.c \
		$(BONUS_DIR)/ft_copy_list_bonus.c \
		$(BONUS_DIR)/execution/ft_heredoc_bonus.c \
		$(BONUS_DIR)/execution/ft_heredoc_utils_bonus.c \
		$(BONUS_DIR)/execution/ft_launcher_bonus.c \
		$(BONUS_DIR)/execution/ft_launcher_utils_bonus.c \
		$(BONUS_DIR)/execution/ft_exec_bonus.c \
		$(BONUS_DIR)/execution/ft_redir_bonus.c \
		$(BONUS_DIR)/execution/ft_exec_utils_bonus.c \
		$(BONUS_DIR)/tokenize/ft_process_input_bonus.c \
		$(BONUS_DIR)/tokenize/ft_charjoin_bonus.c \
		$(BONUS_DIR)/tokenize/ft_isspace_bonus.c \
		$(BONUS_DIR)/tokenize/ft_validate_syntax_bonus.c \
		$(BONUS_DIR)/tokenize/ft_validate_syntax_utils_bonus.c \
		$(BONUS_DIR)/tokenize/ft_validate_parentheses_bonus.c \
		$(BONUS_DIR)/tokenize/ft_create_token_list_bonus.c \
		$(BONUS_DIR)/tokenize/ft_create_token_list_utils_bonus.c \
		$(BONUS_DIR)/tokenize/ft_create_token_list_utils2_bonus.c \
		$(BONUS_DIR)/tokenize/ft_process_token_list_bonus.c \
		$(BONUS_DIR)/tokenize/ft_process_token_list_utils_bonus.c \
		$(BONUS_DIR)/tokenize/ft_process_token_list_utils2_bonus.c \
		$(BONUS_DIR)/tokenize/ft_get_wildcard_list_bonus.c \
		$(BONUS_DIR)/tokenize/ft_get_wildcard_list_utils_bonus.c \
		$(BONUS_DIR)/tokenize/ft_find_next_quote_bonus.c \
		$(BONUS_DIR)/tokenize/ft_build_root_bonus.c \
		$(BONUS_DIR)/tokenize/ft_build_tree_bonus.c \
		$(BONUS_DIR)/tokenize/ft_build_branch_bonus.c \
		$(BONUS_DIR)/tokenize/ft_build_branch_utils_bonus.c \
		$(BONUS_DIR)/tokenize/ft_build_branch_utils2_bonus.c \
		$(BONUS_DIR)/tokenize/ft_free_tree_bonus.c \
		$(BONUS_DIR)/tokenize/ft_is_token_type_bonus.c \
		$(BONUS_DIR)/builtins/ft_env_bonus.c \
		$(BONUS_DIR)/builtins/ft_cd_bonus.c \
		$(BONUS_DIR)/builtins/ft_echo_bonus.c \
		$(BONUS_DIR)/builtins/ft_pwd_bonus.c \
		$(BONUS_DIR)/builtins/ft_export_bonus.c \
		$(BONUS_DIR)/builtins/ft_export_utils_bonus.c \
		$(BONUS_DIR)/builtins/ft_export_ultimate_bonus.c \
		$(BONUS_DIR)/builtins/ft_unset_bonus.c \
		$(BONUS_DIR)/builtins/ft_exit_bonus.c \
		$(BONUS_DIR)/builtins/ft_exit_utils_bonus.c \
		$(BONUS_DIR)/builtins/builtins_utils_bonus.c \

OBJS = $(SRCS:.c=.o)

BONUS_OBJS = $(BONUS_SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

RM = rm -f

VALGRIND = valgrind --leak-check=full --track-fds=yes --trace-children=yes --show-leak-kinds=all --suppressions=.ignore_readline

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME) -lreadline

all: $(NAME) bonus

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME) $(LIBFT)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean
	$(MAKE) all

test: $(NAME)
	$(MAKE) clean
	$(VALGRIND) ./$(NAME)

test_bonus: $(NAME)
	$(MAKE) clean
	$(VALGRIND) ./$(BONUS_NAME)

.PHONY: all clean fclean re bonus
