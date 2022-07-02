# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anaselbaghdadi <anaselbaghdadi@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/10 05:37:53 by arhallab          #+#    #+#              #
#    Updated: 2022/06/28 16:19:22 by anaselbaghd      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC  = token.cpp lexer.cpp location_parser.cpp parser.cpp webserv.cpp conf_file.cpp\
	   server_parse.cpp socket.cpp  tools.cpp request/ClientRequest.cpp\

OBJ  = $(SRC:%.cpp=%.o)

HDR  = $(SRC:%.cpp=%.h)

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
