# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/10 05:37:53 by arhallab          #+#    #+#              #
#    Updated: 2022/06/10 05:57:06 by arhallab         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC  = 

OBJ  = $(SRC:%.cpp=%.o)

HDR  = $(SRC:%.cpp=%.h)

all : $(NAME)

${NAME} : $(OBJ)
	gcc -o $(NAME) $(OBJ)

%.o : %.cpp ${HDR}
	gcc -c -Wall -Wextra -Werror -o $@ $<

clean :
	rm -f $(OBJ)

fclean :
	rm -f $(NAME)