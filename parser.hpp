
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

class Server
{
	public:
		std::vector<int> port;
		std::map<std::string ,Location> locations;
		std::string server_name;
		std::string root;
		bool auto_index;
		std::vector<std::string> index;
		std::map<std::string, std::string> error_pages;
		std::vector<std::string> allowed_methods;
		int body_size_limit;
		std::string cgi;

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
				if (value == "")
				{
					server_name = value;
				}
				else
				{
					std::cerr << "Error : " << value << " is not a valid server name" << std::endl;
					exit(1);
				}
			}
			else if (token == "index")
			{
				this->index = split_white_space(value);
			}
			else if (token == "auto_index")
			{
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
			}
			else if(token == "root")
			{
				if (value == "")
				{
					root = value;
				}
				else
				{
					std::cerr << "Error : " << value << " is not a valid root" << std::endl;
					exit(1);
				}
			}
			else if(token == "body_size_limit")
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
				std::cout << "Error : unknown token" << std::endl;
				exit(1);
			}
		}

		Server(std::vector<std::string> &lines, int start, int end)
		{
			auto_index = false;
			body_size_limit = -1;
			server_name = "";
			root = "";
			cgi = "";
			int i = start;
			// make end stop at locations to parse them separatly
			while(i < end)
			{
				specifyToken(lines[i]);
				i++;
			}
		}
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


