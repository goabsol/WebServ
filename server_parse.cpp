#include "server_parse.hpp"

bool digits_and_colons(std::string &value)
{
	for (size_t i = 0; i < value.size(); i++)
	{
		if (!isdigit(value[i]) && value[i] != '.')
			return false;
	}
	return true;
}

bool isnumber(std::string &value)
{
	for (size_t i = 0; i < value.size(); i++)
	{
		if (!isdigit(value[i]))
			return false;
	}
	return true;
}

std::vector<std::string> split(std::string &value, char c)
{
	std::vector<std::string> result;
	size_t i = 0;
	while (i < value.size())
	{
		size_t j = value.find(c, i);
		if (j == std::string::npos)
			j = value.size();
		result.push_back(value.substr(i, j - i));
		i = j + 1;
	}
	return result;
}

void valid_ipv4(std::string &value, int line)
{
	if (!digits_and_colons(value))
	{
		print_and_exit("Error : invalid ipv4", line);
	}
	std::vector<std::string> parts = split(value, '.');
	if (parts.size() != 4)
	{
		print_and_exit("Error : invalid ipv4", line);
	}
	for (int i = 0; i < 4; i++)
	{
		if (stoi(parts[i]) > 255 || stoi(parts[i]) < 0)
		{
			print_and_exit("Error : invalid ipv4", line);
		}
	}
}

Server_T::Server_T(std::vector<token_T> tokens, size_t &i)
{
	i+=2;
	while (tokens[i].type != RIGHTBRACE)
	{
		if (tokens[i].type == ID)
		{
			if (tokens[i].value == "root")
			{
				i++;
				this->root = tokens[i].value;
			}
			else if (tokens[i].value == "body_size_limit")
			{
				i++;
				try
				{
						this->body_size_limit = std::stoi(tokens[i].value);
				}
				catch(const std::exception& e)
				{
					print_and_exit("Error: body_size_limit must be an integer", tokens[i].line);
				}
				if (this->body_size_limit < 0)
				{
					print_and_exit("Error: body_size_limit must be positive", tokens[i].line);
				}
			}
			else if (tokens[i].value == "allowed_methods")
			{
				i++;
				while(tokens[i].type != SEMICOLON)
				{
					this->allowed_methods.push_back(tokens[i].value);
					i++;
				}
			}
			else if (tokens[i].value == "index")
			{
				while(tokens[i].type != SEMICOLON)
				{
					i++;
					this->index.push_back(tokens[i].value);
				}
			}
			else if (tokens[i].value == "error_page")
			{
				this->error_pages.insert(parse_error_page(tokens,i));
			}
			else if (tokens[i].value == "listen")
			{
				i++;
				if (isnumber(tokens[i].value))
				{
					this->ports.push_back(tokens[i].value);
				}
				else
				{
					valid_ipv4(tokens[i].value, tokens[i].line);
					this->ipv4.push_back(tokens[i].value);
				}
			}
			else if (tokens[i].value == "server_name")
			{
				i++;
				while(tokens[i].type != SEMICOLON)
				{
					this->server_name.push_back(tokens[i].value);
					i++;
				}
			}
			else if (tokens[i].value == "cgi")
			{
				i++;
				while(tokens[i].type != SEMICOLON)
				{
					this->cgi.push_back(tokens[i].value);
					i++;
				}
			}
			else if (tokens[i].value == "autoindex")
			{
				i++;
				if (tokens[i].value == "on")
					this->autoindex = true;
				else if (tokens[i].value == "off")
					this->autoindex = false;
				else
					print_and_exit("Error: autoindex must be on or off", tokens[i].line);
			}
		}
		else if (tokens[i].type == LOCATION)
		{
			i++;
			std::string location_match = tokens[i].value;
			if (locations.find(location_match) != locations.end())
			{
				print_and_exit("Error: location already defined", tokens[i].line);
			}
			this->locations.insert(std::make_pair(location_match, Location_T(tokens, i)));
		}
		else if (tokens[i].type == VALUE)
		{
			print_and_exit("Error: invalid token", tokens[i].line);
		}
		i++;
	}
}

Server_T::Server_T(const Server_T& server)
{
	this->root = server.root;
	this->body_size_limit = server.body_size_limit;
	this->allowed_methods = server.allowed_methods;
	this->index = server.index;
	this->error_pages = server.error_pages;
	this->ports = server.ports;
	this->ipv4 = server.ipv4;
	this->server_name = server.server_name;
	this->cgi = server.cgi;
	this->autoindex = server.autoindex;
	this->locations = server.locations;
}

Server_T& Server_T::operator=(const Server_T& server)
{
	this->root = server.root;
	this->body_size_limit = server.body_size_limit;
	this->allowed_methods = server.allowed_methods;
	this->index = server.index;
	this->error_pages = server.error_pages;
	this->ports = server.ports;
	this->ipv4 = server.ipv4;
	this->server_name = server.server_name;
	this->cgi = server.cgi;
	this->autoindex = server.autoindex;
	this->locations = server.locations;
	return *this;
}