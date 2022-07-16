#include "server_parse.hpp"
#include "cmath"
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

void valid_ipv4(std::string &value, int line)
{
	if (!digits_and_colons(value))
	{
		print_and_exit("invalid ipv4", line);
	}
	std::vector<std::string> parts = split(value, '.');
	if (parts.size() != 4)
	{
		print_and_exit("invalid ipv4", line);
	}
	for (int i = 0; i < 4; i++)
	{
		if (stoi(parts[i]) > 255 || stoi(parts[i]) < 0)
		{
			print_and_exit("invalid ipv4", line);
		}
	}
}

Server_T::Server_T(std::vector<token_T> tokens, size_t &i, parser_T *parser)
{
	this->root = parser->root;
	this->allowed_methods = parser->allowed_methods;
	this->body_size_limit = parser->body_size_limit;
	this->server_name = "";
	this->autoindex = parser->autoindex;
	this->ipv4_set = false;
	this->allowed_methods_set = false;
	this->error_pages = parser->error_pages;
	this->allowed_methods_inh = parser->allowed_methods_set;
	this->body_size_limit_set = false;
	this->autoindex_set = false;
	this->root_set = false;
	this->index_set = false;
	int st = i;
	i+=2;
	while (tokens[i].type != RIGHTBRACE)
	{
		if (tokens[i].type == ID)
		{
			if (tokens[i].value == "root")
			{
				if (this->root_set)
				{
					print_and_exit("root already set", tokens[i].line);
				}
				i++;
				this->root = tokens[i].value;
				this->root_set = true;
			}
			else if (tokens[i].value == "body_size_limit")
			{
				i++;
				if (this->body_size_limit_set)
				{
					print_and_exit("body_size_limit already set", tokens[i].line);
				}
				try
				{
						this->body_size_limit = std::stoi(tokens[i].value);
						this->body_size_limit_set = true;
				}
				catch(const std::exception& e)
				{
					print_and_exit(" body_size_limit must be an integer", tokens[i].line);
				}
				if (this->body_size_limit < 0)
				{
					print_and_exit(" body_size_limit must be positive", tokens[i].line);
				}
			}
			else if (tokens[i].value == "allowed_methods")
			{
				i++;
				if (this->allowed_methods_set)
				{
					print_and_exit("allowed_methods already set", tokens[i].line);
				}
				while(tokens[i].type != SEMICOLON)
				{
					if (!validMethod(tokens[i].value))
					{
						print_and_exit("invalid method", tokens[i].line);
					}
					this->allowed_methods.push_back(tokens[i].value);
					i++;
				}
				this->allowed_methods_set = true;
			}
			else if (tokens[i].value == "index")
			{
				if (this->index_set)
				{
					print_and_exit("index already set", tokens[i].line);
				}
				i++;
				while(tokens[i].type != SEMICOLON)
				{
					this->index.push_back(tokens[i].value);
					i++;
				}
				this->index_set = true;
			}
			else if (tokens[i].value == "error_page")
			{
				std::pair<int, std::string> tmp_error = parse_error_page(tokens,i, "error page ");
				this->error_pages[tmp_error.first] = tmp_error.second;

			}
			else if (tokens[i].value == "listen")
			{
				i++;
				if (this->ports.size() > 0)
				{
					print_and_exit("ports already set", tokens[i].line);
				}
				while(tokens[i].type == VALUE)
				{
					std::vector<std::string> addr = split(tokens[i].value, ':');
					if (this->ipv4_set)
					{
						print_and_exit("only one listen address allowed", tokens[i].line);
					}
					if (addr.size() == 1)
					{
						if (!isnumber(addr[0]))
						{
							print_and_exit(" port must be an integer", tokens[i].line);
						}
						this->ports.push_back(std::make_pair("0.0.0.0",std::stoi(addr[0])));
					}
					else if (addr.size() == 2)
					{
						if (this->ports.size())
						{
							print_and_exit("Cannot listen on multiple addresses", tokens[i].line);
						}
						valid_ipv4(addr[0], tokens[i].line);
						if (!isnumber(addr[1]))
						{
							print_and_exit(" port must be an integer", tokens[i].line);
						}
						this->ports.push_back(std::make_pair(addr[0], stol(addr[1])));
						this->ipv4_set = true;
					}
					else
					{
						print_and_exit(" invalid listen", tokens[i].line);
					}
					i++;
				}
			}
			else if (tokens[i].value == "server_name")
			{
				i++;
				if (this->server_name != "")
				{
					print_and_exit("server names already set", tokens[i].line);
				}
				this->server_name = tokens[i].value;
				i++;
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
				if (this->autoindex_set)
				{
					print_and_exit("autoindex already set", tokens[i].line);
				}
				if (tokens[i].value == "on")
					this->autoindex = true;
				else if (tokens[i].value == "off")
					this->autoindex = false;
				else
					print_and_exit(" autoindex must be on or off", tokens[i].line);
				this->autoindex_set = true;
			}
		}
		else if (tokens[i].type == LOCATION)
		{
			i++;
			std::string location_match = tokens[i].value;
			if (locations.find(location_match) != locations.end())
			{
				print_and_exit(" location already defined", tokens[i].line);
			}
			this->locations.insert(std::make_pair(location_match, Location_T(tokens, i, this)));
		}
		else if (tokens[i].type == VALUE)
		{
			print_and_exit(" invalid token", tokens[i].line);
		}
		i++;
	}
	if (this->server_name == "")
	{
		print_and_exit("server name not set", st);
	}
	else if (this->ports.size() == 0)
	{
		print_and_exit("no ports set", st);
	}
}

Server_T::Server_T() : root("/"), body_size_limit(0), autoindex(true)
{
}

Server_T::Server_T(const Server_T& server)
{
	this->root = server.root;
	this->body_size_limit = server.body_size_limit;
	this->allowed_methods = server.allowed_methods;
	this->index = server.index;
	this->error_pages = server.error_pages;
	this->ports = server.ports;
	this->server_name = server.server_name;
	this->cgi = server.cgi;
	this->autoindex = server.autoindex;
	this->locations = server.locations;
	this->ipv4_set = server.ipv4_set;
	this->allowed_methods_set = server.allowed_methods_set;
	this->body_size_limit_set = server.body_size_limit_set;
	this->index_set = server.index_set;
	this->autoindex_set = server.autoindex_set;
	this->allowed_methods_inh = server.allowed_methods_inh;
}

Server_T& Server_T::operator=(const Server_T& server)
{
	this->root = server.root;
	this->body_size_limit = server.body_size_limit;
	this->allowed_methods = server.allowed_methods;
	this->index = server.index;
	this->error_pages = server.error_pages;
	this->server_name = server.server_name;
	this->cgi = server.cgi;
	this->autoindex = server.autoindex;
	this->locations = server.locations;
	this->ports = server.ports;
	this->ipv4_set = server.ipv4_set;
	this->allowed_methods_set = server.allowed_methods_set;
	this->body_size_limit_set = server.body_size_limit_set;
	this->index_set = server.index_set;
	this->autoindex_set = server.autoindex_set;
	this->allowed_methods_inh = server.allowed_methods_inh;
	return *this;
}

Server_T::~Server_T()
{
}