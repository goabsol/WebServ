# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/10 05:37:53 by arhallab          #+#    #+#              #
#    Updated: 2022/07/02 23:31:49 by arhallab         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC  = token.cpp lexer.cpp location_parser.cpp parser.cpp webserv.cpp conf_file.cpp\
	   server_parse.cpp socket.cpp  tools.cpp request/ClientRequest.cpp workshop.cpp\

OBJ  = $(SRC:%.cpp=%.o)

HDR  = $(SRC:%.cpp=%.hpp)

FLG  = -fsanitize=address -g3 -std=c++98

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

rclean : all clean
