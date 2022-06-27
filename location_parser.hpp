#pragma once

#include "token.hpp"
#include "tools.hpp"
#include "server_parse.hpp"

class Server_T;

class Location_T
{
	public:
		Location_T(std::vector<token_T> &tokens, size_t &i, Server_T *server);
		Location_T();
		Location_T(const Location_T& location);
		Location_T& operator=(const Location_T& location);
		std::string root;
		std::vector<std::string> cgi;
		std::vector<std::string> allowed_methods;
		int body_size_limit;
		std::vector<std::string> index;
		bool autoindex;
		std::string upload_store;
		std::map<std::string, Location_T> locations;
};