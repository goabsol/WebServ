# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/10 05:37:53 by arhallab          #+#    #+#              #
#    Updated: 2022/07/17 22:19:47 by arhallab         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC  = token.cpp lexer.cpp location_parser.cpp parser.cpp webserv.cpp conf_file.cpp\
	   server_parse.cpp socket.cpp  tools.cpp request/ClientRequest.cpp workshop.cpp\

OBJ  = $(SRC:%.cpp=%.o)

HDR  = $(SRC:%.cpp=%.hpp) mimetypes.hpp

FLG  = -std=c++98 -Wall -Werror -Wextra

all : $(NAME)

${NAME} : $(OBJ)
	c++ $(FLG) $(OBJ) -o $(NAME)

%.o : %.cpp $(HDR)
	c++ $(FLG) -c $< -o $@

re: fclean all

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

rclean : all clean
