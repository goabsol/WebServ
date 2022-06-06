#pragma once

#include "tools.hpp"

class Conf;
class Location;
class Server
{
	public:
		std::vector<int> port;
		bool body_size_limit_set;
		bool root_set;
		bool auto_index_set;
		std::vector<Location> locations;
		std::vector<std::string> server_name;
		std::string root;
		bool auto_index;
		std::vector<std::string> index;
		std::map<std::string, std::string> error_pages;
		std::vector<std::string> allowed_methods;
		std::vector<std::string> tmp_allowed_methods;
		std::vector<std::string> new_error_pages;
		int body_size_limit;
		std::vector<std::string> cgi;
		void specifyToken(std::string &line);

		Server(std::vector<std::string> &lines, int start, int end, Conf &global_config);
};

