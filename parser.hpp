
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


class Server
{
	public:
		std::vector<std::string> port;
		std::map<std::string ,Location> locations;
		std::string server_name;
		std::string type;
		std::string root;
		bool auto_index;
		std::vector<std::string> index;
		std::map<int, std::string>  error_page;
		std::string cgi;
};

class Block
{
	public:
		std::string root;
		bool auto_index;
		std::string index;
		std::string error_page;
		std::vector<Server> servers;
};



class Location : Server
{
	
};