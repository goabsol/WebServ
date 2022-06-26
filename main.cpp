#include "lexer.hpp"
#include "parser.hpp"


void fancy_print(std::string &value, int type)
{
	std::cout << GREEN << "value: |" << RESET_COLOR << std::left << std::setw(30)<< value << GREEN  <<"| type: |" << RESET_COLOR << token_type_to_string(type) << std::endl;
}

void conf_parse(lexer_T *lexer)
{
	std::vector<token_T> tokens;
	token_T tok;
	while ((tok = lexer_get_token(lexer)).type != END_OF_FILE)
	{
		tokens.push_back(tok);
	}
	parser_T parser = parser_T(tokens);
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
