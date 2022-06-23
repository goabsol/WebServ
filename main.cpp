#include "lexer.hpp"
#include "parser.hpp"

void conf_parse(lexer_T *lexer)
{
	// parser_T parser = parser_T(lexer);
	// AST_T *root = parser_parse(parser);
	// std::cout << root << '\n';
	// std::cout << parser.token.value << '\n';
	// parser = parser_T(lexer);
	// std::cout << parser.token.value << '\n';
	token_T tok;
	while ((tok = lexer_get_token(lexer)).type != END_OF_FILE)
	{
		std::cout << GREEN << "Token : " << RESET_COLOR << std::left <<std::setw(15) << tok.value << GREEN   << " Token type :" << RESET_COLOR << token_type_to_string(tok.type) << std::endl;
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage : ./webserv <config_file>" << std::endl;
	}
	std::ifstream file(av[1]);
	if (!file.is_open())
	{
		std::cerr << "Error : config file not found" << std::endl;
		exit(1);
	}
	std::string src;
	while (file.good())
	{
		std::string line;
		std::getline(file, line);
		src += line + "\n";
	}
	lexer_T lexer = lexer_T(src);
	conf_parse(&lexer);
	file.close();
	// while(1);
	return 0;
}
