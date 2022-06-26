#include "location_parser.hpp"


Location_T::Location_T()
{
	root = "";
	cgi = std::vector<std::string>();
	allowed_methods = std::vector<std::string>();
	body_size_limit = 0;
	autoindex = false;
	upload_store = "";
}

Location_T::Location_T(const Location_T& location)
{
	root = location.root;
	cgi = location.cgi;
	allowed_methods = location.allowed_methods;
	body_size_limit = location.body_size_limit;
	autoindex = location.autoindex;
	upload_store = location.upload_store;
	locations = location.locations;
}

Location_T& Location_T::operator=(const Location_T& location)
{
	root = location.root;
	cgi = location.cgi;
	allowed_methods = location.allowed_methods;
	body_size_limit = location.body_size_limit;
	autoindex = location.autoindex;
	upload_store = location.upload_store;
	locations = location.locations;
	return *this;
}

Location_T::Location_T(std::vector<token_T> tokens, size_t &i)
{
	i+=2;
	while(tokens[i].type != RIGHTBRACE)
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
			print_and_exit("Error: cannot have nested locations", tokens[i].line);

		}
		else if (tokens[i].type == VALUE)
		{
			print_and_exit("Error: invalid token", tokens[i].line);
		}
		i++;
	}
}
