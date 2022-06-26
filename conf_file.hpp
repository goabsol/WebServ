#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include "lexer.hpp"
#include "parser.hpp"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define RESET_COLOR "\033[0m"




void	print_and_exit(const std::string &msg, size_t line);

