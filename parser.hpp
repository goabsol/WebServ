#pragma once

#include "lexer.hpp"

typedef struct PARSER_STRUCT
{
	lexer_T* lexer;
	token_T* token;

} parser_T;


parser_T* init_parser(lexer_T* lexer);