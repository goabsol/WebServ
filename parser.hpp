
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
		std::map<int, std::string>  error_pages;
		std::string body_size_limit;
		std::string cgi;
};

class Conf
{
	public:
		std::string root;
		std::vector<std::string> allowed_methods;
		std::vector<std::string> index;
		std::map<std::string, std::string> error_pages;
		int body_size_limit;
		std::vector<std::string> split_white_space(std::string &line)
		{
			std::vector<std::string> words;
			std::string word = "";
			for (int i = 0; i < line.length(); i++)
			{
				if (!iswspace(line[i]))
				{
					word += line[i];
				}
				else if (iswspace(line[i]) && word != "")
				{
					words.push_back(word);
					word = "";
				}
			}
			if (word != "")
			{
				words.push_back(word);
			}
			return words;
		}
		bool string_is_digit(std::string str)
		{
			for (int i = 0; i < str.length(); i++)
			{
				if (!isdigit(str[i]))
				{
					return false;
				}
			}
			return true;
		}
		bool validMethod(std::string &method)
		{
			if (method != "POST" && method != "GET" && method != "HEAD" && method != "PUT" && method != "DELETE")
			{
				return false;
			}
			return true;
		}
		void specifyToken(std::string line)
		{
			
			std::string token = line.substr(0, line.find(' '));
			std::string value = line.substr(line.find(' ') + 1);
			if (token == value)
			{
				std::cerr << "Error : " << token << " is not valid" << std::endl;
				exit(1);
			}
			if (token == "root")
			{
				if (this->root == "")
					this->root = value;
				else
				{
					std::cout << "Error: root is specified more than once" << std::endl;
					exit(1);
				}

			}
			else if (token == "index")
			{
				if (this->index.size() == 0)
				{
					this->index = split_white_space(value);
				}
				else
				{
					std::cout << "Error: index is specified more than once" << std::endl;
					exit(1);
				}
			}
			else if (token == "error_page")
			{
				std::string error_code = value.substr(0, value.find(' '));
				if (error_code == value.substr(value.find(' ') + 1))
				{
					std::cerr << "Error code : " << error_code << " is not valid" << std::endl;
					exit(1);
				}
				if (!string_is_digit(error_code))
				{
					std::cerr << "Error: error code invalid" << std::endl;
					exit(1);
				}
				std::string error_page = value.substr(value.find(' ') + 1);
				if (this->error_pages.find(error_code) == this->error_pages.end())
					this->error_pages[error_code] = error_page;
				else
				{
					std::cout << "Error: error_page is specified more than once" << std::endl;
					exit(1);
				}
			}
			else if (token == "body_size_limit")
			{
				if (this->body_size_limit == -1)
				{
					this->body_size_limit = stoi(value);
					if (this->body_size_limit < 0)
					{
						std::cerr << "Error: body_size_limit is invalid" << std::endl;
						exit(1);
					}
				}
				else
				{
					std::cout << "Error: body_size_limit is specified more than once" << std::endl;
					exit(1);
				}
			}
			else if (token == "allowed_methods")
			{
				if (this->allowed_methods.size() == 0)
				{
					this->allowed_methods = split_white_space(value);
					for(int i = 0; i < this->allowed_methods.size(); i++)
					{
						if (!validMethod(this->allowed_methods[i]))
						{
							std::cerr << "Error :allowed_methods: " << this->allowed_methods[i] << " is not valid" << std::endl;
							exit(1);
						}
					}
				}
				else
				{
					std::cout << "Error: allowed_methods is specified more than once" << std::endl;
					exit(1);
				}
			}
			else
			{
				std::cout << "Error: unknown token" << std::endl;
				exit(1);
			}
			
		}
		Conf(std::vector<std::string> &lines, int startOfServer)
		{
			this->root = "";
			this->body_size_limit = -1;
			int i = 0;
			while (i < startOfServer)
			{
				specifyToken(lines[i]);
				i++;
			}
		}
};


