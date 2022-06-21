#include "lexer.hpp"

lexer_T *init_lexer(std::string src)
{
	lexer_T *lexer = new lexer_T;
	lexer->src = src;
	lexer->i = 0;
	lexer->src_size = src.size();
	lexer->c = src[lexer->i];
	return lexer;
}

void lexer_advance(lexer_T *lexer)
{
	if (lexer->i < lexer->src_size && lexer->c != '\0')
	{
		lexer->i++;
		lexer->c = lexer->src[lexer->i];
	}
}

char lexer_peek(lexer_T *lexer, int i)
{
	return lexer->src[std::min((size_t)lexer->i + i, lexer->src_size)];
}

token_T *lexer_advance_with(lexer_T *lexer, token_T *token)
{
	lexer_advance(lexer);
	return token;
}

token_T *lexer_advance_current(lexer_T *lexer, int type)
{
	std::string value = "";
	value += lexer->c;
	token_T *token = init_token(value, type);
	lexer_advance(lexer);

	return token;
}

void lexer_skip_whitespace(lexer_T *lexer)
{
	while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r')
	{
		lexer_advance(lexer);
	}
}
token_T *lexer_parse_id(lexer_T *lexer)
{
	std::string value;
	while (isalpha(lexer->c) || lexer->c == '_')
	{
		value += lexer->c;
		lexer_advance(lexer);
	}
	return init_token(value, ID);
}

token_T *lexer_parse_number(lexer_T *lexer)
{
	std::string value;
	while (isdigit(lexer->c))
	{
		value += lexer->c;
		lexer_advance(lexer);
	}
	return init_token(value, NUMBER);
}

token_T *lexer_get_token(lexer_T *lexer)
{
	while (lexer->c != '\0')
	{
		lexer_skip_whitespace(lexer);
		if (lexer->i >= lexer->src_size || lexer->c == '\0')
		{
			return init_token("", END_OF_FILE);
		}
		if (isalpha(lexer->c) || lexer->c == '_')
			return (lexer_parse_id(lexer));

		if (isdigit(lexer->c))
			return (lexer_parse_number(lexer));

		switch (lexer->c)
		{
		case ':':
		{
			return (lexer_advance_with(lexer, init_token(":", COLON)));
		}
		break;
		case '{':
			return lexer_advance_current(lexer, LEFTBRACE);
		case '}':
			return lexer_advance_current(lexer, RIGHTBRACE);
		case ';':
			return lexer_advance_current(lexer, SEMICOLON);
		case '\0':break;
		default : std::cerr << "Unknown character: " << lexer->c << std::endl; exit(1);
		}
	}

	return init_token(0, END_OF_FILE);
}
