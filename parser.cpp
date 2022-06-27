#include "parser.hpp"

parser_T::parser_T()
{
}

parser_T::parser_T(std::vector<token_T> tokens)
{
	this->root = "";
	this->body_size_limit = 0;
	for(size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].type == ID)
		{
			if (tokens[i].value == "root")
			{
				i++;
				if (this->root != "")
				{
					print_and_exit("root already defined", tokens[i].line);
				}
				this->root = tokens[i].value;
			}
			else if (tokens[i].value == "body_size_limit")
			{
				i++;
				if (this->body_size_limit != 0)
				{
					print_and_exit("body_size_limit already defined", tokens[i].line);
				}
				try
				{
						this->body_size_limit = std::stoi(tokens[i].value);
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
				if (this->allowed_methods.size() != 0)
				{
					print_and_exit("allowed_methods already defined", tokens[i].line);
				}
				while(tokens[i].type != SEMICOLON)
				{
					this->allowed_methods.push_back(tokens[i].value);
					i++;
				}
			}
			else if (tokens[i].value == "index")
			{
				if (this->index.size() != 0)
				{
					print_and_exit("index already defined", tokens[i].line);
				}
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
			else
			{
				print_and_exit(" unknown token: " + tokens[i].value, tokens[i].line);
			}
		}
		else if (tokens[i].type == SERVER)
		{
			servers.push_back(Server_T(tokens, i));
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
}
parser_T& parser_T::operator=(const parser_T& parser)
{
	this->root = parser.root;
	this->body_size_limit = parser.body_size_limit;
	this->allowed_methods = parser.allowed_methods;
	this->index = parser.index;
	this->error_pages = parser.error_pages;
	this->servers = parser.servers;
	return *this;
}