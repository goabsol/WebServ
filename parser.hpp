#pragma once

#include "lexer.hpp"
#include "parsing_tools.hpp"
#include "server_parse.hpp"

class parser_T
{
	public:
		token_T token;
		parser_T(std::vector<token_T> tokens);
		parser_T();
		parser_T(const parser_T& parser);
		parser_T& operator=(const parser_T& parser);
		std::vector<std::string> allowed_methods;
		std::vector<std::string> index;
		std::map<int, std::string> error_pages;
		int body_size_limit;
		std::string root;
		std::vector<Server_T> servers;
};


parser_T parse_file(int ac, char **av);