#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include "../tools.hpp"
#include "../server_parse.hpp"

class ClientRequest
{
	public:

		ClientRequest();
		ClientRequest( ClientRequest const & src );
		ClientRequest (int, Server_T &);
		~ClientRequest();
		bool getHasError();
		bool getIsDone();
		std::string getError();
		std::string getData();
		int getSocket();
		void setData(std::string line);
		ClientRequest &	operator=( ClientRequest const & rhs );
		void checkLineValidity(std::string line);
		void parseRequest(std::string &line);
		void storeRequest();
		bool isValidURI(std::string &);
		void setIsDone(bool);
		bool getConnectionClosed();
		bool locationExists(std::string &request);
		bool autorised_method(std::string &method);
		std::map<std::string, std::string> getRequestFields();
		Server_T &getServer();
		void clearData();
		int Socket;
		std::string method;
		std::string requestURI;
		std::string httpVersion;
		std::map<std::string, std::string> requestFields;
		std::string body;
		std::string data;
		bool hasError;
		bool needHost;
		std::string errorMessage;
		int requestPosition;
		bool isDone;
		bool closeConnection;
		Server_T server;
		Location_T current_location;
		std::string current_location_path;
};

std::ostream &			operator<<( std::ostream & o, ClientRequest const & i );