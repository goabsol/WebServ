#include "location_parser.hpp"


Location_T::Location_T()
{
	char tmp[256];
    getcwd(tmp, 256);
	root = tmp;
	cgi = std::vector<std::string>();
	allowed_methods = std::vector<std::string>();
	autoindex = false;
	upload_store = "";
}

Location_T::Location_T(const Location_T& location)
{
	root = location.root;
	cgi = location.cgi;
	allowed_methods = location.allowed_methods;
	autoindex = location.autoindex;
	upload_store = location.upload_store;
	locations = location.locations;
	redirection = location.redirection;
	index = location.index;
	
	autoindex_set = location.autoindex_set;
	upload_store_set = location.upload_store_set;
	allowed_methods_set = location.allowed_methods_set;
	root_set = location.root_set;
	index_set = location.index_set;
	redirection_set = location.redirection_set;

	allowed_methods_inh = location.allowed_methods_inh;
}

Location_T& Location_T::operator=(const Location_T& location)
{
	root = location.root;
	cgi = location.cgi;
	allowed_methods = location.allowed_methods;
	autoindex = location.autoindex;
	upload_store = location.upload_store;
	locations = location.locations;
	redirection = location.redirection;
	index = location.index;

	autoindex_set = location.autoindex_set;
	upload_store_set = location.upload_store_set;
	allowed_methods_set = location.allowed_methods_set;
	root_set = location.root_set;
	index_set = location.index_set;
	redirection_set = location.redirection_set;
	allowed_methods_inh = location.allowed_methods_inh;
	return *this;
}

Location_T::Location_T(std::vector<token_T> &tokens, size_t &i, Server_T *server)
{
	this->allowed_methods = server->allowed_methods;
	this->autoindex = server->autoindex;
	this->index = server->index;
	this->root = server->root;
	this->redirection = std::make_pair(0, "");

	this->autoindex_set = false;
	this->upload_store_set = false;
	this->allowed_methods_set = false;
	this->root_set = false;
	this->index_set = false;
	this->redirection_set = false;
	this->allowed_methods_inh = server->allowed_methods_inh;
	i+=2;
	while(tokens[i].type != RIGHTBRACE)
	{
		if (tokens[i].type == ID)
		{
			if (tokens[i].value == "root")
			{
				if (this->root_set)
				{
					print_and_exit("Error: root already set", tokens[i].line);
				}
				i++;
				this->root = tokens[i].value;
				this->root_set = true;
			}
			else if (tokens[i].value == "allowed_methods")
			{
				i++;
				if (this->allowed_methods_set)
				{
					print_and_exit("Error: allowed_methods already set", tokens[i].line);
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
				i++;
				if (this->index_set)
				{
					print_and_exit("Error: index already set", tokens[i].line);
				}
				while(tokens[i].type != SEMICOLON)
				{
					this->index.push_back(tokens[i].value);
					i++;
				}
				this->index_set = true;
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
					print_and_exit("Error: autoindex already set", tokens[i].line);
				}
				if (tokens[i].value == "on")
					this->autoindex = true;
				else if (tokens[i].value == "off")
					this->autoindex = false;
				else
					print_and_exit(" autoindex must be on or off", tokens[i].line);
				this->autoindex_set = true;
			}
			else if (tokens[i].value == "return")
			{
				if (this->redirection_set)
				{
					print_and_exit("Error: redirection already set", tokens[i].line);
				}
				this->redirection = parse_error_page(tokens, i, "redirection");
				this->redirection_set = true;
			}
			else
			{
				print_and_exit("Error: invalid location parameter", tokens[i].line);
			}
		}
		else if (tokens[i].type == LOCATION)
		{
			print_and_exit(" cannot have nested locations", tokens[i].line);

		}
		else if (tokens[i].type == VALUE)
		{
			print_and_exit(" invalid token", tokens[i].line);
		}
		i++;
	}
	if (this->root == "")
	{
		print_and_exit("Error: root not set", tokens[i].line);
	}
}
