
#include "tools.hpp"
std::vector<std::string> split_white_space(std::string &line)
{
	std::vector<std::string> words;
	std::string word = "";
	for (int i = 0; i < line.length(); i++)
	{
		if (!iswspace(line[i]))
		{
			word += line[i];
		}
		else if (iswspace(line[i]) && word != "")
		{
			words.push_back(word);
			word = "";
		}
	}
	if (word != "")
	{
		words.push_back(word);
	}
	return words;
}

int location_start(std::vector<std::string> &lines, int start)
{
	int i = start;
	while(i < lines.size() && lines[i].substr(0, lines[i].find(" ")) != "location" && lines[i] != "server")
	{
		i++;
	}
	return i;
}

int find_end_of_location(std::vector<std::string> &lines, int start)
{
	int i = start;
	while(i < lines.size() && lines[i] != "location" && lines[i] != "server")
	{
		i++;
	}
	if (i == start)
	{
		std::cerr << "Error : invalid location bock" << std::endl;
		exit(1);
	}
	return (i);
}

bool string_is_digit(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (!isdigit(str[i]))
		{
			return false;
		}
	}
	return true;
}

bool validMethod(std::string &method)
{
	if (method != "POST" && method != "GET" && method != "HEAD" && method != "PUT" && method != "DELETE")
	{
		return false;
	}
	return true;
}
