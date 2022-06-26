
#include "tools.hpp"
std::vector<std::string> split_white_space(std::string &line)
{
	std::vector<std::string> words;
	std::string word = "";
	for (size_t i = 0; i < line.length(); i++)
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

int countChr(std::string &str, char c)
{
	int count = 0;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == c)
			count++;
	}
	return count;
}

int location_start(std::vector<std::string> &lines, int start)
{
	size_t i = start;
	while(i < lines.size() && lines[i].substr(0, lines[i].find(" ")) != "location" && lines[i] != "server")
	{
		i++;
	}
	return i;
}

int find_end_of_location(std::vector<std::string> &lines, int start)
{
	size_t i = start;
	while(i < lines.size() && lines[i] != "location" && lines[i] != "server")
	{
		i++;
	}
	if (i == (size_t)start)
	{
		std::cerr << "Error : invalid location bock" << std::endl;
		exit(1);
	}
	return (i);
}

bool string_is_digit(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
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


std::vector<std::string> validMethods(std::string &methods)
{
	std::vector<std::string> tmp = split_white_space(methods), ret;

	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (!validMethod(tmp[i]) || (find(ret.begin(), ret.end(), tmp[i]) != ret.end()))
		{
			std::cerr << "Error: " << tmp[i] << " allowed method is not valid or duplicated" << std::endl;
			exit(1);
		}
		ret.push_back(tmp[i]);
	}
	return ret;
}

void *memeset(void *s, int c, size_t n) //it's a memset function but in memeset
{
    char *p = (char *)s;
    while (n--)
        *p++ = c;
    return s;
}