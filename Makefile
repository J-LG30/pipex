NAME = pipex
CC = @cc
FLAGS = -Wall -Wextra -Werror #-fsanitize=address -static-libsan -g

SRCS = pipex.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT_A) -o $(@)

$(LIBFT_A):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

clean:
	@rm -f $(OBJS)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
