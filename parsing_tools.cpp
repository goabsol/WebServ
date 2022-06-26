#include "parsing_tools.hpp"

void	print_and_exit(const std::string &msg, size_t line)
{
	std::cerr << "Error: " << msg << " at line " << line << std::endl;
	exit(1);
}

