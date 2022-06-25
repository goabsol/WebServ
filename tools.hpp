#pragma once


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
#include "Location.hpp"
#include "Server.hpp"
#include "parser.hpp"

std::vector<std::string> split_white_space(std::string &line);

int location_start(std::vector<std::string> &lines, int start);

int find_end_of_location(std::vector<std::string> &lines, int start);

bool string_is_digit(std::string str);

std::vector<std::string> validMethods(std::string &method);

void *memeset(void *s, int c, size_t n);
