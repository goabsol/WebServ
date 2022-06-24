#include "lexer.hpp"


lexer_T::lexer_T()
{
	this->i = 0;
	this->src = "";
	this->src_size = src.size();
	this->c = src[0];
	this->depth = 0;
	this->last_type = START_OF_FILE;
	this->line = 1;
}

lexer_T::lexer_T(std::string newsrc)
{
	this->i = 0;
	this->src = newsrc;
	this->src_size = src.size();
	this->c = src[0];
	this->depth = 0;
	this->last_type = START_OF_FILE;
	this->line = 1;
}


void lexer_advance(lexer_T *lexer)
{
	if (lexer->i < lexer->src_size && lexer->c != '\0')
	{
		lexer->i++;
		lexer->c = lexer->src[lexer->i];
	}
}

// char lexer_peek(lexer_T *lexer, int i)
// {
// 	return lexer->src[std::min((size_t)lexer->i + i, lexer->src_size)];
// }

token_T lexer_advance_with(lexer_T *lexer, token_T token)
{
	lexer_advance(lexer);
	return token;
}

token_T lexer_advance_current(lexer_T *lexer, int type)
{
	std::string value = "";
	value += lexer->c;
	token_T token = token_T(value, type, lexer->line);
	lexer_advance(lexer);

	return token;
}

int is_whitespace(char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void lexer_skip_whitespace(lexer_T *lexer)
{
	while (is_whitespace(lexer->c))
	{
		if (lexer->c == '\n')
		{
			lexer->line++;
		}
		lexer_advance(lexer);
	}
}

int valid_string(char c)
{
	return isalnum(c) || c == '_' || c == '/' || c == '.' || c == ':';
}

int valid_id(std::string str)
{
	// later
	(void)str;
	return 1;
}

int taken_chars(char c)
{
	return c == ';' || c == '{' || c == '}' || c == '#';
}

token_T lexer_parse_id(lexer_T *lexer)
{
	std::string value;
	while (!is_whitespace(lexer->c) && !taken_chars(lexer->c))

	{
		value += lexer->c;
		lexer_advance(lexer);
	}
	if (lexer->last_type == ID || lexer->last_type == VALUE)
	{
		lexer->last_type = VALUE;
		return token_T(value, VALUE, lexer->line);
	}
	if (lexer->last_type == LOCATION)
	{
		lexer->last_type = LOCATION_MATCH;
		return token_T(value, LOCATION_MATCH, lexer->line);
	}
	if (lexer->last_type == SERVER || lexer->last_type == LOCATION_MATCH)
	{
		print_and_exit("Cannot have inline server or location", lexer->line);
	}
	if (value == "server")
	{
		if (lexer->depth != 0)
			print_and_exit("Error : server is not allowed inside another server", lexer->line);
		lexer->last_type = SERVER;
		return token_T(value, SERVER, lexer->line);
	}
	else if (value == "location")
	{
		if (lexer->depth == 0)
			print_and_exit("Error : location is not allowed outside of a server", lexer->line);
		lexer->last_type = LOCATION;
		return token_T(value, LOCATION, lexer->line);
	}
	if (!valid_id(value))
	{
		print_and_exit("Error : invalid id", lexer->line);
	}
	lexer->last_type = ID;
	return token_T(value, ID, lexer->line);
}


token_T lexer_get_token(lexer_T *lexer)
{
	while (lexer->c != '\0')
	{
		lexer_skip_whitespace(lexer);
		if (lexer->i >= lexer->src_size || lexer->c == '\0')
		{
			if (lexer->depth != 0 || (lexer->last_type != SEMICOLON && lexer->last_type != RIGHTBRACE))
				print_and_exit("Error : missing closing bracket", lexer->line - 1);
			return token_T("", END_OF_FILE, lexer->line);
		}
		if (valid_string(lexer->c))
		{
			return (lexer_parse_id(lexer));
		}
		switch (lexer->c)
		{
		break;
		case '{':
		{
			if (lexer->last_type != SERVER && lexer->last_type != LOCATION_MATCH)
			{
				if (lexer->last_type == LOCATION)
					print_and_exit("Error: location needs a path. Use / to match all paths", lexer->line);
				print_and_exit("Error : only a Server or a Location can have an opening bracket.", lexer->line);
			}
			lexer->last_type = LEFTBRACE;
			lexer->depth++;
			return lexer_advance_current(lexer, LEFTBRACE);
		}
		case '}':
		{
			if (lexer->depth == 0 || (lexer->last_type != SEMICOLON && lexer->last_type != RIGHTBRACE))
			{
				print_and_exit("Error : invalid closing bracket", lexer->line);
			}
			lexer->last_type = RIGHTBRACE;
			lexer->depth--;
			return lexer_advance_current(lexer, RIGHTBRACE);
		}
		case ';':
		{
			if (lexer->last_type != VALUE && (lexer->last_type == ID || lexer->last_type == SERVER || lexer->last_type == LOCATION || lexer->last_type == LOCATION_MATCH))
			{
				print_and_exit("Error : Expected valid expression before semicolon.", lexer->line);
			}
			lexer->last_type = SEMICOLON;
			return lexer_advance_current(lexer, SEMICOLON);
		}
		case '#':
		{
			while (lexer->c != '\n')
			{
				lexer_advance(lexer);
			}
			continue;
		}
		case '\0':break;
		default : std::cerr << "Unknown character: '" << lexer->c << "' at line : " << lexer->line << std::endl; exit(1);
		}
	}
	if (lexer->depth != 0 || (lexer->last_type != SEMICOLON && lexer->last_type != RIGHTBRACE))
		print_and_exit("Error : missing closing bracket", lexer->line);
	return token_T(0, END_OF_FILE, lexer->line);
}


lexer_T& lexer_T::operator=(const lexer_T& lexer)
{
	this->i = lexer.i;
	this->src = lexer.src;
	this->src_size = lexer.src_size;
	this->c = lexer.c;
	return *this;
}

lexer_T::lexer_T(const lexer_T& lexer)
{
	this->i = lexer.i;
	this->src = lexer.src;
	this->src_size = lexer.src_size;
	this->c = lexer.c;
}