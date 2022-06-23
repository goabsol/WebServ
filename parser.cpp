#include "parser.hpp"

parser_T::parser_T()
{
}

parser_T::parser_T(lexer_T *lexer)
{
	this->lexer = lexer;
	this->token = lexer_get_token(lexer);
}

token_T parser_T::parser_eat(int type)
{
	if (this->token.type != type)
	{
		std::cerr << "Parser: Unexpected token: `" <<  token_type_to_string(this->token.type) << "` Was expecting : "<< token_type_to_string(type) << '\n';
		exit(1);
	}

	this->token = lexer_get_token(this->lexer);
	return (this->token);
}

// AST_T *parser_T::parser_parse(parser_T *parser)
// {
// 	(void) parser;
// 	return init_ast(A_NOOP);
// }

// AST_T *parser_T::parser_parse_compound(parser_T *parser)
// {
// 	// AST_T *compound = init_ast(AST_COMPOUND);
// 	while(parser->token->type != END_OF_FILE)
// 	{
// 		/* AST_T * child =  */ parser_parse(parser);
// 	}

// 	return (init_ast(A_NOOP));
// }