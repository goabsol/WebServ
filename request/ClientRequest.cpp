/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bagh <ael-bagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:31:16 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/26 10:47:42 by ael-bagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientRequest.hpp"

const std::string tmp[] = {"GET",
						   "POST",
						   "PUT",
						   "DELETE",
						   "HEAD",
						   "OPTIONS",
						   "CONNECT",
						   "TRACE"};
const std::vector<std::string> v_methods(tmp,tmp+8);
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientRequest::ClientRequest()
{
}

ClientRequest::ClientRequest( const ClientRequest & src )
{
}

ClientRequest::ClientRequest(int socket) : Socket(socket), data(""), requestPosition(0), hasError(false), isDone(false)
{
	std::cout << socket << " " << this->Socket << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ClientRequest::~ClientRequest()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ClientRequest &				ClientRequest::operator=( ClientRequest const & rhs )
{
	if ( this != &rhs )
	{
		this->Socket = rhs.Socket;
		this->method = rhs.method;
		this->requestURI = rhs.requestURI;
		this->httpVersion = rhs.httpVersion;
		this->requestFields = rhs.requestFields;
		this->body = rhs.body;
		this->data = rhs.data;
		this->hasError = rhs.hasError;
		this->needHost = rhs.needHost;
		this->errorMessage = rhs.errorMessage;
		this->requestPosition = rhs.requestPosition;
		this->isDone = rhs.isDone;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ClientRequest const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

bool ClientRequest::getHasError()
{
	return this->hasError;
}
std::string ClientRequest::getError()
{
	return this->errorMessage;
}
std::string ClientRequest::getData()
{
	return this->data;
}
int ClientRequest::getSocket()
{
	return this->Socket;
}
bool ClientRequest::getIsDone()
{
	return this->isDone;
}

/* ************************************************************************** */

void ClientRequest::parseRequest()
{
	std::string line;
	
	while(this->data.find("\r\n\r\n") != std::string::npos)
	{
		if (this->data.find("\r\n") != std::string::npos)
		{
			if (this->requestPosition < 2)
				this->requestPosition++;
			line = this->data.substr(0, this->data.find("\r\n"));
			this->checkLineValidity(line);
		}
		this->data = this->data.substr(this->data.find("\r\n") + 2);
		//parse the line
	}
	this->requestPosition = 3;
}

void ClientRequest::checkLineValidity(std::string line)
{
	if (this->requestPosition == 1)
	{
		std::vector<std::string> requestline = split_white_space(line);
		if (requestline.size() != 3 || countChr(line, ' ') != 2)
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line wrong number of parameters";
			return ;
		}
		if (std::find(v_methods.begin(), v_methods.end(), requestline[0]) == v_methods.end())
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line method not valid";
			return ;
		}
		else
		{
			this->method = requestline[0];
		}
		this->requestURI = requestline[1];
		this->httpVersion = requestline[2];
		// {
		// 	hasError = 1;
		// 	errorMessage = "Error: Request method is wrong";
		// 	return ;
		// }
		// im reading about what an URI should be like so meh
		//if (requestline[1] )
	}
	else if (requestPosition == 2)
	{
		char *l = strdup(line.c_str());
		std::string p(strtok(l, ": "));
		std::string v(strtok(l, ": "));
		requestFields[p]= v;
		std::cout << p << " aha " << v << std::endl;
	}
	else
	{

	}
}


void ClientRequest::storeRequest()
{
	char buffer[1024];
	memeset(buffer, 0, 1024);
    int bytes_read;
    std::vector<std::string> lines;
    bytes_read = recv(this->Socket, buffer, 1024, 0);
	std::cout << bytes_read << std::endl;
	if (bytes_read > 0)
	{
		this->data += std::string(buffer);
	}
	else
	{
		this->hasError = true;
		this->errorMessage = "Error: recv() failed";
	}
    if (this->data.find("\r\n") != std::string::npos)
    {
		std::cout << "found" << std::endl;
		this->parseRequest();
		isDone = true;
    }
	std::cout << this->data << std::endl;
}