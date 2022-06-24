#include "token.hpp"


token_T::token_T()
{
	this->value = "";
	this->type = -1;
}
token_T::token_T(std::string value, int type)
{
	this->value = value;
	this->type = type;
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