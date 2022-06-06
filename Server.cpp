#include "Server.hpp"
#include "tools.hpp"
void Server::specifyToken(std::string &line)
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
		catch (...)
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
	else if (token == "root")
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
	else if (token == "body_size_limit")
	{
		if (this->body_size_limit_set == false)
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
		for (int i = 0; i < tmp.size(); i++)
		{
			if (!validMethod(tmp[i]) || (find(this->tmp_allowed_methods.begin(), this->tmp_allowed_methods.end(), tmp[i]) != this->tmp_allowed_methods.end()))
			{
				std::cerr << "Error: " << tmp[i] << " allowed method is not valid or duplicated" << std::endl;
				exit(1);
			}
			this->tmp_allowed_methods.push_back(tmp[i]);
		}
		for (int i = 0; i < this->tmp_allowed_methods.size(); i++)
		{
			if (find(this->allowed_methods.begin(), this->allowed_methods.end(), this->tmp_allowed_methods[i]) == this->allowed_methods.end())
			{
				this->allowed_methods.push_back(this->tmp_allowed_methods[i]);
			}
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
		if (find(new_error_pages.begin(), new_error_pages.end(), error_code) == new_error_pages.end())
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
	else if (token == "cgi")
	{
		this->cgi = split_white_space(value);
	}
	else
	{
		std::cout << "Error : unknown token : " << token << std::endl;
		exit(1);
	}
}

Server::Server(std::vector<std::string> &lines, int start, int end, Conf &global_config)
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
	end = location_start(lines, start);
	while (i < end)
	{
		specifyToken(lines[i]);
		i++;
	}
	int end_of_location = end;
	if (lines[end].substr(0, lines[end].find(" ")) == "location")
	{
		
		while (end_of_location < lines.size() && lines[end_of_location] != "server")
		{
			std::string location_value = lines[end].substr(lines[end].find(" ") + 1);
			if (location_value == "location")
			{
				std::cerr << "Error: location is not valid" << std::endl;
				exit(1);
			}
			end_of_location = location_start(lines, end + 1);
			Location location = Location(lines, end + 1, end_of_location, this, location_value);

			locations.push_back(location);
			end = end_of_location;
		}
	}
}