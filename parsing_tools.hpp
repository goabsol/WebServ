#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm> 

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define RESET_COLOR "\033[0m"

#define ID 0
#define LEFTBRACE 1
#define RIGHTBRACE 2
#define SEMICOLON 4
#define END_OF_FILE 5
#define NUMBER 6
#define VALUE 7
#define START_OF_FILE 8
#define SERVER 9
#define LOCATION 10
#define LOCATION_MATCH 11


void	print_and_exit(const std::string &msg, size_t line);

