#pragma once

#include "token.hpp"

typedef struct LEXER_STRUCT
{
	std::string src;
	size_t src_size;
	char c;
	unsigned int i;

} lexer_T;

char lexer_peek(lexer_T *lexer, int i);

void lexer_advance(lexer_T* lexer);

lexer_T *init_lexer(std::string src);

void lexer_next(lexer_T *lexer);

void lexer_skip_whitespace(lexer_T *lexer);

token_T *lexer_get_token(lexer_T *lexer);

token_T *lexer_parse_id(lexer_T *lexer);

token_T *lexer_parse_number(lexer_T *lexer);

token_T *lexer_advance_with(lexer_T *lexer, token_T* token);

token_T *lexer_advance_current(lexer_T *lexer, int type);