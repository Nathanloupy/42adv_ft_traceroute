# Makefile for ft_traceroute

CC = cc
CCFLAGS = -Wall -Wextra -Werror

NAME = ft_traceroute

INCLUDES = -I./include/ -I./lpyp/include -I./lpyp/libft

LPYP_DIR = lpyp
LPYP_LIB = $(LPYP_DIR)/lpyp.a
LIBFT_LIB = $(LPYP_DIR)/libft/libft.a

SRCS_MAIN = src/main.c

SRCS_CORE = src/parser.c

SRCS_UTILS = src/utils/time.c

OBJS_MAIN = $(SRCS_MAIN:.c=.o)
OBJS_CORE = $(SRCS_CORE:.c=.o)
OBJS_UTILS = $(SRCS_UTILS:.c=.o)

OBJS = $(OBJS_MAIN) $(OBJS_CORE) $(OBJS_UTILS)

%.o: %.c
	@$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

all: $(LPYP_LIB) $(NAME)

$(LPYP_LIB):
	@make -s --no-print-directory -C $(LPYP_DIR)

$(NAME): $(OBJS) $(LPYP_LIB)
	@$(CC) $(CCFLAGS) $(INCLUDES) $(OBJS) $(LPYP_LIB) $(LIBFT_LIB) -o $(NAME) -lm

clean:
	@rm -f $(OBJS)
	@make -s --no-print-directory -C $(LPYP_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -s --no-print-directory -C $(LPYP_DIR) fclean

re: fclean all

run: all
	@./$(NAME) $(ip)

.PHONY: all clean fclean re run