#pragma once

#include "lexer.hpp"
#include "AST.hpp"

class parser_T
{
	public:
		lexer_T *lexer;
		token_T token;
		parser_T(lexer_T *lexer);
		parser_T();
		token_T parser_eat(int type);

		// AST_T *parser_parse(parser_T *parser);

		// AST_T *parser_parse_compound(parser_T *parser);
};
