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
		case COLON: return "COLON";
		case SEMICOLON: return "SEMICOLON";
	}
	return "END_OF_FILE";
}