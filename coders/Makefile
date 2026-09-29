# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/06 14:22:57 by pmuzika           #+#    #+#              #
#    Updated: 2026/09/28 13:47:07 by pmuzika          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRC =	main.c \
		utils.c \
		init.c \
		monitor.c \
		parsing.c \
		compile.c \
		scheduler.c \
		threads.c \
		heap.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re