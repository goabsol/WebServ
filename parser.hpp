
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

class Location
{
	
};


class Conf
{
	public:
		std::string root;
		std::vector<std::string> allowed_methods;
		std::vector<std::string> index;
		std::map<std::string, std::string> error_pages;
		int body_size_limit;
		
		
		
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
					if (!string_is_digit(value))
					{
						std::cerr << "Error: body_size_limit is not valid" << std::endl;
						exit(1);
					}
					try{

						this->body_size_limit = stoi(value);
					}
					catch(...)
					{
						std::cerr << "invalid body size" << std::endl;
						exit(1);
					}
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
				std::vector<std::string> tmp;

				if (this->allowed_methods.size() == 0)
				{
					tmp = split_white_space(value);
					for(int i = 0; i < tmp.size(); i++)
					{
						if (!validMethod(tmp[i]) || find(this->allowed_methods.begin(), this->allowed_methods.end(), tmp[i]) != this->allowed_methods.end())
						{
							std::cerr << "Error: " << tmp[i] << " allowed method is not valid or duplicated" << std::endl;
							exit(1);
						}
						this->allowed_methods.push_back(tmp[i]);
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
				std::cout << "Error : unknown token : " << token << std::endl;
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

class Server
{
	public:
		std::vector<int> port;
		bool body_size_limit_set;
		bool root_set;
		bool auto_index_set;
		std::map<std::string ,Location> locations;
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

		void specifyToken(std::string &line)
		{
			std::string token = line.substr(0, line.find(" "));
			std::string value = line.substr(line.find(" ") + 1);
			if (token == value)
			{
				std::cerr << "Error : " << token << " is not valid" << std::endl;
				exit(1);
			}
			else if (token == "listen")
			{
				if (!string_is_digit(value))
				{
					std::cerr << "Error : " << value << " is not a valid port" << std::endl;
					exit(1);
				}
				try
				{
					int port_number = std::stoi(value);
					port.push_back(port_number);
				}
				catch(...)
				{
					std::cerr << "invalid port number " << std::endl;
					exit(1);
				}
			}
			else if (token == "server_name")
			{
				this->server_name = split_white_space(value);
			}
			else if (token == "index")
			{
				this->index = split_white_space(value);
			}
			else if (token == "auto_index")
			{
				if (auto_index_set == true)
				{
					std::cerr << "Error : auto_index is specified more than once" << std::endl;
					exit(1);
				}
				if (value != "on" && value != "off")
				{
					std::cerr << "Error : " << value << " is not a valid value for auto_index" << std::endl;
					exit(1);
				}
				if (value == "on")
				{
					auto_index = true;
				}
				else
				{
					auto_index = false;
				}
				auto_index_set = true;
			}
			else if(token == "root")
			{
				if (root_set == false)
				{
					root = value;
					root_set = true;
				}
				else
				{
					std::cerr << "Error : " << value << " is not a valid root" << std::endl;
					exit(1);
				}
			}
			else if(token == "body_size_limit")
			{
				if (this->body_size_limit_set == false)
				{
					if (!string_is_digit(value))
					{
						std::cerr << "Error: body_size_limit is not valid" << std::endl;
						exit(1);
					}
					try{

						this->body_size_limit = stoi(value);
					}
					catch(...)
					{
						std::cerr << "invalid body size" << std::endl;
						exit(1);
					}
					if (this->body_size_limit < 0)
					{
						std::cerr << "Error: body_size_limit is invalid" << std::endl;
						exit(1);
					}
					this->body_size_limit_set = true;
				}
				else
				{
					std::cout << "Error: body_size_limit is specified more than once" << std::endl;
					exit(1);
				}
			}
			else if (token == "allowed_methods")
			{
				std::vector<std::string> tmp;
				tmp = split_white_space(value);
				for(int i = 0; i < tmp.size(); i++)
				{
					if (!validMethod(tmp[i]) || (find(this->tmp_allowed_methods.begin(),this->tmp_allowed_methods.end(), tmp[i]) != this->tmp_allowed_methods.end()))
					{
						std::cerr << "Error: " << tmp[i] << " allowed method is not valid or duplicated" << std::endl;
						exit(1);
					}
					this->tmp_allowed_methods.push_back(tmp[i]);
				}
				for(int i = 0; i < this->tmp_allowed_methods.size(); i++)
				{
					if (find(this->allowed_methods.begin(), this->allowed_methods.end(), this->tmp_allowed_methods[i]) == this->allowed_methods.end())
					{
						this->allowed_methods.push_back(this->tmp_allowed_methods[i]);
					}
				}
			}
			else if(token == "error_page")
			{
				std::string error_code = value.substr(0, value.find(' '));
				if (!string_is_digit(error_code))
				{
					std::cerr << "Error: error code invalid" << std::endl;
					exit(1);
				}
				std::string error_page = value.substr(value.find(' ') + 1);
				if (find(new_error_pages.begin(),new_error_pages.end(), error_code) == new_error_pages.end())
				{
					this->error_pages[error_code] = error_page;
					new_error_pages.push_back(error_code);
				}
				else
				{
					std::cout << "Error: error_page is specified more than once" << std::endl;
					exit(1);
				}
			}
			else if(token == "cgi")
			{
				this->cgi = split_white_space(value);
			}
			else
			{
				std::cout << "Error : unknown token : " << token << std::endl;
				exit(1);
			}
		}

		Server(std::vector<std::string> &lines, int start, int end, Conf &global_config)
		{
			auto_index = false;
			body_size_limit = global_config.body_size_limit;
			root = global_config.root;
			body_size_limit_set = false;
			auto_index_set = false;
			root_set = false;
			allowed_methods = global_config.allowed_methods;
			index = global_config.index;
			error_pages = global_config.error_pages;
			int i = start;
			// make "end" stop at locations to parse them separatly
			while(i < end)
			{
				specifyToken(lines[i]);
				i++;
			}
		}
};

