#include "token.hpp"


token_T::token_T()
{
	this->value = "";
	this->type = -1;
}
token_T::token_T(std::string value, int type, int line)
{
	this->value = value;
	this->type = type;
	this->line = line;
}


std::string token_type_to_string(int type)
{
	switch(type)
	{
		case ID: return "ID";
		case LEFTBRACE: return "LEFTBRACE";
		case RIGHTBRACE: return "RIGHTBRACE";
		case SEMICOLON: return "SEMICOLON";
		case VALUE: return "VALUE";
		case SERVER: return "SERVER";
		case LOCATION: return "LOCATION";
		case LOCATION_MATCH : return "LOCATION_MATCH";
	}
	return "END_OF_FILE";
}

token_T::token_T(const token_T& tok)
{
	this->value = tok.value;
	this->type = tok.type;
}

token_T& token_T::operator=(const token_T& tok)
{
	this->value = tok.value;
	this->type = tok.type;
	return *this;
}

std::pair<int,std::string> parse_error_page(std::vector<token_T> tokens,  size_t &i)
{
	int code;
	std::string page;
	i++;
	try {
	code = std::stoi(tokens[i].value);
	}
	catch (const std::exception& e)
	{
		print_and_exit("Error: error_pages must be an integer", tokens[i].line);
	}
	if (code < 0 || code > 599)
	{
		print_and_exit("Error: error_pages must be between 0 and 599", tokens[i].line);
	}
	i++;
	if (tokens[i].type != VALUE)
	{
		print_and_exit("Error: error_page code must be followed by a page name", tokens[i].line);
	}
	page = tokens[i].value;
	return (std::make_pair(code, page));
}