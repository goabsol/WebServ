
#pragma once

#include "tools.hpp"
#include "server_parse.hpp"
#include "request/ClientRequest.hpp"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "mimetypes.hpp"
//#define FILE true
#define DIRECTORY false
#define ON true
#define OFF false



std::string craftResponse(ClientRequest &request, int status_code = 200, std::string message = "OK");
std::string getFileExtension(std::string const &file_name);
bool getRequestedResource(std::string &resource);
bool getResourceType(std::string &path);
bool indexInDir(std::vector<std::string> &indexes, std::string &dir, std::string &found);
bool getAutoIndex(Location_T &location);
bool gotCGI(ClientRequest &request, std::string &file_name);
std::string makeautoindex(std::string &root, std::string &dir);


bool emptyDir(std::string &dir);

