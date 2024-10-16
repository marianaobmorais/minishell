NAME = minishell
#NAME_BONUS = minishell_bonus

SRCS_DIR = ./srcs
#BONUS_DIR = ./bonus

LIBFT = ./libft/libft.a
LIBFT_DIR = ./libft

SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/ft_error_handler.c \
		$(SRCS_DIR)/builtins/ft_env.c \
		$(SRCS_DIR)/builtins/ft_cd.c \

#BONUS_SRCS = $(BONUS_DIR)/

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

RM = rm -f

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --supressions=external_func_leaks.supp


$(NAME): $(LIBFT) $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)

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
