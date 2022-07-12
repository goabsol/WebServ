#pragma once

#include <sys/time.h>
#include <ctime>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <map>
#include <fstream>
#include <fcntl.h>
#include <exception>

#define SOCKET int


std::vector<std::string> split_white_space(std::string &line);

int location_start(std::vector<std::string> &lines, int start);

int find_end_of_location(std::vector<std::string> &lines, int start);

bool string_is_digit(std::string str);

std::vector<std::string> validMethods(std::string &method);

void *memeset(void *s, int c, size_t n);

int countChr(std::string &str, char c);

void	print_and_exit(const std::string &msg, size_t line);

std::vector<std::string> split(std::string &value, char c);

bool validMethod(std::string &method);

void trimwspace(std::string &str);

bool validURI(std::string &uri);

void hexaToAscii(std::string &str);

class http_error_exception : public std::exception
{
public:
	virtual ~http_error_exception() throw();
	int code;
	std::string message;
	http_error_exception(int code, const std::string message);
	virtual const char* what() const throw();
};
