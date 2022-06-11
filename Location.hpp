#pragma once


#include "tools.hpp"
class Server;
class Location
{
	public:
		bool body_size_limit_set;
		bool root_set;
		bool auto_index_set;
		bool auto_index;
		bool allowed_methods_set;
		std::string location_value;
		std::vector<std::string> allowed_methods;
		std::vector<std::string> tmp_allowed_methods;
		int body_size_limit;
		std::vector<std::string> cgi;
		std::vector<std::string> index;
		std::string root;
		Location(std::vector<std::string> &lines, int start, int end, Server *parent, std::string value);
		void splitTokens(std::string &line);
};