NAME = parser
FILE = file.conf
SRC = $(wildcard *.cpp)

OBJ = $(SRC:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++11

SANITIZE = -fsanitize=address -g

$(NAME): $(OBJ)
	g++  $(OBJ) $(FLAGS) -o$(NAME) $(SANITIZE)
	rm -f $(OBJ)


%.o: %.cpp
	g++ -c $(FLAGS) $< -o $@


clean:
	rm -f $(NAME) $(OBJ)

re:
	make clean
	make
run:
	make
	./$(NAME) $(FILE)