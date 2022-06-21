#include "lexer.hpp"

void conf_parse(std::string src)
{
	lexer_T *lexer = init_lexer(src);
	token_T *tok = 0;

	while ((tok = lexer_get_token(lexer))->type != END_OF_FILE)
	{
		std::cout << tok->value << " " << tok->type << std::endl;
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
	conf_parse(src);
	file.close();
	return 0;
}
