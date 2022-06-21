#include "token.hpp"


token_T* init_token(std::string value, int type)
{
	token_T* token = new token_T;
	token->value = value;
	token->type = type;
	return token;
}

