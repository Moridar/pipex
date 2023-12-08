# Makefile for pipex
# Auother: Bobbie Syvasalmi
# Version 1.0
# Date: 07/12/2023

NAME = pipex

LIB = libft/libft.a
LIBDIR = ./libft
SRC = pipexhack.c
SRC_BONUS = pipex_bonus.c
OBJ = $(SRC:%.c=%.o)
OBJ_BONUS = $(SRC_BONUS:%.c=%.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror

$(NAME): all
	
.phony: all
all: $(OBJ)

bonus: $(OBJ_BONUS)

%.o: %.c $(LIB)
	$(CC) $(CFLAGS) $< -c
	$(CC) $(CFLAGS) $@ -L$(LIBDIR) -lft -o $(NAME)

$(LIB):
	make -C $(LIBDIR)

clean:
	rm -rf *.o
	make -C $(LIBDIR) clean

fclean: clean
	rm $(NAME)
	rm $(LIB)
	
re: fclean all

.phony: all clean fclean re