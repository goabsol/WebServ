#include "parser.hpp"
void Conf::specifyToken(std::string line)
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
			try
			{

				this->body_size_limit = stoi(value);
			}
			catch (...)
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
			for (int i = 0; i < tmp.size(); i++)
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
Conf::Conf(std::vector<std::string> &lines, int startOfServer)
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