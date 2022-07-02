#pragma once

#include "token.hpp"
#include "tools.hpp"
#include "parser.hpp"
#include "location_parser.hpp"
class parser_T;
class Location_T;

class Server_T
{
	public:
		Server_T(const Server_T& server);
		Server_T(std::vector<token_T> tokens, size_t &i, parser_T *parser);
		Server_T();
		~Server_T();
		Server_T& operator=(const Server_T& server);

		std::vector<std::string> allowed_methods;
		std::vector<std::string> index;
		std::map<int, std::string> error_pages;
		int body_size_limit;
		std::string root;
		std::vector<std::string> cgi;
		std::string server_name;
		std::vector<std::pair<std::string, long> >ports;
		std::map<std::string, Location_T> locations;
		bool autoindex;

		bool ipv4_set;
		bool allowed_methods_set;
		bool allowed_methods_inh;
		bool index_set;
		bool body_size_limit_set;
		bool root_set;
		bool autoindex_set;
};
