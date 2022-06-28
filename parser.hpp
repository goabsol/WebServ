#pragma once

#include "lexer.hpp"
#include "server_parse.hpp"

class Server_T;

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
		bool autoindex;

		bool allowed_methods_set;
		bool index_set;
		bool body_size_limit_set;
		bool root_set;
		bool autoindex_set;
};

parser_T parse_file(int ac, char **av);