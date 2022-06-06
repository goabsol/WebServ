#pragma once



#include "tools.hpp"


class Conf
{
	public:
		std::string root;
		std::vector<std::string> allowed_methods;
		std::vector<std::string> index;
		std::map<std::string, std::string> error_pages;
		int body_size_limit;
		
		
		
		void specifyToken(std::string line);
		Conf(std::vector<std::string> &lines, int startOfServer);
};


