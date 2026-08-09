# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmuzika <pmuzika@student.42malaga.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/06 14:22:57 by pmuzika           #+#    #+#              #
#    Updated: 2026/08/07 21:17:14 by pmuzika          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRC = coders/codexion.c \
		coders/codexion_utils.c

OBJ = $(SRC:.c=.o)

# all:
# 	cc -Wall -Wextra -Werror -pthread coders/codexion.c coders/codexion_utils.c -o codexion


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