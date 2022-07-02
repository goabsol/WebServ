#include "parser.hpp"

parser_T::parser_T()
{
}

parser_T::parser_T(std::vector<token_T> tokens)
{
	this->root = "";
	this->body_size_limit = 1024 * 1024;
	this->root_set = false;
	this->body_size_limit_set = false;
	this->autoindex = false;
	this->autoindex_set = false;
	this->allowed_methods_set = false;
	for(size_t i = 0; i < tokens.size(); i++)
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
				this->error_pages.insert(parse_error_page(tokens,i, "error page "));
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
			else
			{
				print_and_exit(" unknown token: " + tokens[i].value, tokens[i].line);
			}
		}
		else if (tokens[i].type == SERVER)
		{
			servers.push_back(Server_T(tokens, i, this));
		}
	}
}


parser_T::parser_T(const parser_T& parser)
{
	this->root = parser.root;
	this->body_size_limit = parser.body_size_limit;
	this->allowed_methods = parser.allowed_methods;
	this->index = parser.index;
	this->error_pages = parser.error_pages;
	this->servers = parser.servers;
	this->autoindex = parser.autoindex;
	this->allowed_methods_set = parser.allowed_methods_set;
	this->index_set = parser.index_set;
	this->body_size_limit_set = parser.body_size_limit_set;
	this->root_set = parser.root_set;
	this->autoindex_set = parser.autoindex_set;

}
parser_T& parser_T::operator=(const parser_T& parser)
{
	this->root = parser.root;
	this->body_size_limit = parser.body_size_limit;
	this->allowed_methods = parser.allowed_methods;
	this->index = parser.index;
	this->error_pages = parser.error_pages;
	this->servers = parser.servers;
	this->autoindex = parser.autoindex;
	this->allowed_methods_set = parser.allowed_methods_set;
	this->index_set = parser.index_set;
	this->body_size_limit_set = parser.body_size_limit_set;
	this->root_set = parser.root_set;
	this->autoindex_set = parser.autoindex_set;
	return *this;
}