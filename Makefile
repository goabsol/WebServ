# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/10 05:37:53 by arhallab          #+#    #+#              #
#    Updated: 2022/06/26 10:40:15 by arhallab         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC  = token.cpp lexer.cpp location_parser.cpp parser.cpp webserv.cpp conf_file.cpp\
	   server_parse.cpp socket.cpp  tools.cpp request/ClientRequest.cpp\

OBJ  = $(SRC:%.cpp=%.o)

HDR  = $(SRC:%.cpp=%.h)

FLG  = -fsanitize=address -g3

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
