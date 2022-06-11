#include "Location.hpp"
#include "tools.hpp"
Location::Location(std::vector<std::string> &lines, int start, int end, Server *parent, std::string value)
{
	auto_index_set = false;
	body_size_limit_set = false;
	root_set = false;
	allowed_methods_set = false;
	auto_index = parent->auto_index;
	body_size_limit = parent->body_size_limit;
	root = parent->root;
	allowed_methods = parent->allowed_methods;
	cgi = parent->cgi;
	index = parent->index;
	location_value = value;
	int i = start;
	while(i < end)
	{
		splitTokens(lines[i]);
		i++;
	}
}
void Location::splitTokens(std::string &line)
{
	std::string token = line.substr(0, line.find(" "));
	std::string value = line.substr(line.find(" ") + 1);
	if (token == value)
	{
		std::cerr << "Error : invalid location block" << std::endl;
		exit(1);
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
		if(allowed_methods_set)
		{
			std::cerr << "Allowed methods specified more than once.\n";
			exit(1);
		}
		this->allowed_methods = validMethods(value);
		allowed_methods_set = true;
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