#include "parser.hpp"

std::string replace_white_spaces(std::string const & str)
{
	std::string new_str = "";
	for (int i = 0; i < str.length(); i++)
	{
		if (!iswspace(str[i]))
		{
			new_str += str[i];
		}
		else if (iswspace(str[i]) && new_str.back() != ' ')
		{
			new_str += ' ';
		}
	}
	return new_str;
}


int main(int argc, char **argv, char **env)
{
	std::ifstream indata;
	std::string line;
	if (argc != 2)
	{
		std::cerr << "Usage : ./webserv <config_file>" << std::endl;
		return (1);
	}
	indata.open(argv[1]);
	if (!indata)
	{
		std::cerr << "Error: File could not be opened." << std::endl;
		return (1);
	}
	std::string lines;
	while (getline(indata, line))
	{
		size_t ind;
		if ((ind = line.find('#')) != std::string::npos)
			line.erase(ind);
		line = replace_white_spaces(line);
		if (line[0] == ' ')
			line.erase(0, 1);
		if (line != "")
		{
			lines += " " + line;
		}
	}
		lines.erase(0,1);
		std::cout << lines << std::endl;
}