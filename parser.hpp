
#include <ctime>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <map>
#include <fstream>
class Location
{
	
};

class Server
{
	public:
		std::vector<std::string> port;
		std::map<std::string ,Location> locations;
		std::string server_name;
		std::string type;
		std::string root;
		bool auto_index;
		std::vector<std::string> index;
		std::map<int, std::string>  error_page;
		std::string cgi;
};

class Conf
{
	public:
		std::string root;
		std::string allowed_methods;
		std::string index;
		std::vector<std::string> error_pages;
		std::string body_size_limit;
		void specifyToken(std::string line)
		{
			
			std::string token = line.substr(0, line.find(' '));
			if (token == "root")
			{
				if (this->root == "")
					this->root = line.substr(line.find(' ') + 1, line.size() - line.find(' '));
				else
				{
					std::cout << "Error: root is specified more than once" << std::endl;
					exit(1);
				}

			}
			else if (token == "index")
			{
				if (this->index == "")
					this->index = line.substr(line.find(' ') + 1, line.size() - line.find(' '));
				else
				{
					std::cout << "Error: index is specified more than once" << std::endl;
					exit(1);
				}
			}
			else if (token == "error_page")
			{
				// need to remove duplicates later
				error_pages.push_back(line.substr(line.find(' ') + 1, line.size() - line.find(' ')));
			}
			else if (token == "allowed_methods")
			{
				if (this->allowed_methods == "")
					this->allowed_methods = line.substr(line.find(' ') + 1, line.size() - line.find(' '));
				else
				{
					std::cout << "Error: allowed_methods is specified more than once" << std::endl;
					exit(1);
				}
			}
			else if (token == "body_size_limit")
			{
				if (this->body_size_limit == "")
					this->body_size_limit = line.substr(line.find(' ') + 1, line.size() - line.find(' '));
				else
				{
					std::cout << "Error: body_size_limit is specified more than once" << std::endl;
					exit(1);
				}
			}
			else
			{
				std::cerr << "Invalid token" << std::endl;
				exit(1);
			}
		}
		Conf(std::vector<std::string> &lines, int startOfServer)
		{
			this->root = "";
			this->allowed_methods = "";
			this->index =  "";
			this->body_size_limit = "";
			int i = 0;
			while (i < startOfServer)
			{
				specifyToken(lines[i]);
				i++;
			}
		}
};


