#include "token.hpp"


token_T* init_token(std::string value, int type)
{
	token_T* token = new token_T;
	token->value = value;
	token->type = type;
	return token;
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