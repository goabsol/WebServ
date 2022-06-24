#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>

class ClientRequest
{
	public:

		ClientRequest();
		ClientRequest( ClientRequest const & src );
		ClientRequest (int &);
		~ClientRequest();
		bool getHasError();
		bool getIsDone();
		std::string getError();
		std::string getData();
		void setData(std::string line);
		ClientRequest &	operator=( ClientRequest const & rhs );

		void parseRequest();
		void getRequest();

	private:
		int Socket;
		std::string method;
		std::string requestURI;
		std::string httpVersion;
		std::map<std::string, std::string> requestFields;
		std::string body;
		std::string data;
		bool hasError;
		std::string errorMessage;
		bool isDone;
};

std::ostream &			operator<<( std::ostream & o, ClientRequest const & i );
