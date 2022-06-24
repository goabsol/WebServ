#pragma once

#include "includes.h"

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


class  token_T
{
	public:
		std::string value;
		int type;
		token_T(std::string, int type);
		token_T();
		token_T(const token_T& tok);
		token_T& operator=(const token_T& tok);
};


std::string token_type_to_string(int type);