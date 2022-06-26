#pragma once

#include "parsing_tools.hpp"



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