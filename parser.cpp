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

int findStart(std::vector<std::string> &lines)
{
	int i = 0;
	while ((i < lines.size()) && (lines[i] != "server:"))
	{
		i++;
	}
	if (i >= lines.size())
	{
		std::cerr << "Error : no servers in config file" << std::endl;
		exit(1);
	}
	return i;
}

void parse_conf(std::vector<std::string> &lines)
{
	int start_of_servers = 0;
	std::vector<Server> servers;
	start_of_servers = findStart(lines);
	Conf global_config = Conf(lines, start_of_servers);
	std::map<std::string, std::string>::iterator it;
	// for (it = global_config.error_pages.begin(); it != global_config.error_pages.end(); it++)
	// {
	// 	std::cout << it->first << " " << it->second << std::endl;
	// }
	// std::cout << "root: " << global_config.root << std::endl;
	// for (int i = 0; i < global_config.allowed_methods.size(); i++)
	// {
	// 	std::cout << global_config.allowed_methods[i] << '\n';
	// }
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
		std::cerr << "Error: Could not open configuration file." << std::endl;
		return (1);
	}
	std::vector<std::string> lines;
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
			lines.push_back(line);
			// std::cout << line << std::endl;
		}
	}
	parse_conf(lines);
}