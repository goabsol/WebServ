# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/10 05:37:53 by arhallab          #+#    #+#              #
#    Updated: 2022/06/25 10:29:08 by arhallab         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC  = webserv.cpp tools.cpp request/ClientRequest.cpp

OBJ  = $(SRC:%.cpp=%.o)

HDR  = $(SRC:%.cpp=%.h)

FLG  = 

all : $(NAME)

${NAME} : $(OBJ)
	c++ $(FLG) $(OBJ) -o $(NAME)

%.o : %.cpp
	c++ $(FLG) -c $< -o $@

re: fclean all

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)