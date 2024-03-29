# Makefile for pipex
# Auother: Bobbie Syvasalmi
# Version 1.0
# Date: 07/12/2023

NAME = pipex

LIB = libft/libft.a
LIBDIR = ./libft
SRC = pipex.c pipex_utils.c
SRC_BONUS = pipex_bonus.c pipex_utils.c
OBJ = $(SRC:%.c=%.o)
OBJ_BONUS = $(SRC_BONUS:%.c=%.o)
BONUS = .bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -lft -o $(NAME)

.phony: all
all: $(NAME)

bonus: $(BONUS)

$(BONUS): $(OBJ_BONUS)
	$(CC) $(CFLAGS) $(OBJ_BONUS) -L$(LIBDIR) -lft -o $(NAME)
	@touch .bonus

%.o: %.c $(LIB)
	$(CC) $(CFLAGS) $< -c
	

$(LIB):
	make -C $(LIBDIR)

clean:
	rm -f .bonus
	rm -rf *.o
	make -C $(LIBDIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIB)
	
re: fclean all

.phony: all clean fclean re