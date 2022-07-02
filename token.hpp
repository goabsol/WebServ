#pragma once

#include "tools.hpp"

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
		token_T(std::string, int type, int line);
		token_T();
		token_T(const token_T& tok);
		int line;
		token_T& operator=(const token_T& tok);
};


std::string token_type_to_string(int type);

std::pair<int,std::string> parse_error_page(std::vector<token_T> tokens,  size_t &i);

class http_error_exception : public std::exception
{
public:
	int code;
	std::string message;
	http_error_exception(int code, const std::string message);
	const char* what() const noexcept;
};
